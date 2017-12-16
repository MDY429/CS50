from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp
import datetime
from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    # Select data we need to update.
    rows = db.execute("SELECT symbol, sum(shares) as total_shares FROM history where id_username = :id_username group by symbol having sum(shares) > 0",\
                        id_username = session["user_id"]
                        )
    
    # Update the symbol, current price and total.
    for c in range(len(rows)):
        a = db.execute("UPDATE history SET price_now = :price_now, total = :total where id_username = :id_username and symbol = :symbol",\
                        price_now = "{:,.2f}".format(lookup(rows[c]["symbol"])["price"]),\
                        total = "{:,.2f}".format(lookup(rows[c]["symbol"])["price"]*rows[c]["total_shares"]),\
                        id_username = session["user_id"],\
                        symbol = rows[c]["symbol"]
                        )
    
    # Select again the data we need.
    rows = db.execute("SELECT symbol,symbol_name, price_now, sum(shares) AS total_shares ,total AS total_price \
                        FROM history where id_username = :id_username group by symbol having sum(shares) > 0", id_username = session["user_id"])

    # Select the user cash.
    usercash = db.execute("SELECT cash FROM users where id = :id", id = session["user_id"])
    
    # Count the total property.
    total = 0
    for row in rows:
        total += (float(row["price_now"]))*row["total_shares"]
    total += usercash[0]["cash"]
    
    return render_template("index.html", rows = rows, cash = usd(usercash[0]["cash"]), total = usd(total))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "POST":

        # ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("missing symbol")
        elif not request.form.get("shares") or not request.form.get("shares").isdigit():
            return apology("invalid shares")
        elif int(request.form.get("shares")) < 1:
            return apology("invalid shares")
        else:
            findsymbol = lookup(request.form.get("symbol"))
            if not findsymbol:
                return apology("invalid symbol")
                
            rows = db.execute("SELECT cash FROM users where id = :id", id =session["user_id"])
            if findsymbol["price"] * float(request.form.get("shares")) > float(rows[0]["cash"]):
                return apology("can't afford")

            # Insert the new data into history.
            rows = db.execute("INSERT INTO history (id_username, symbol, symbol_name, shares, price, transacted)\
                            VALUES(:id_username, :symbol, :symbol_name, :shares, :price, :transacted)",\
                            id_username =session["user_id"],\
                            symbol = findsymbol["symbol"],\
                            symbol_name = findsymbol["name"],\
                            shares = request.form.get("shares"),\
                            price = "{:,.2f}".format(findsymbol["price"]),\
                            transacted = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                            )
            
            # Update user's cash.
            rows = db.execute("UPDATE users SET cash = cash - :total where id = :id",\
                                total = findsymbol["price"] * int(request.form.get("shares")), \
                                id =session["user_id"]
                                )
            
            flash('Bought!')
            return redirect(url_for("index"))
            
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    rows = db.execute("SELECT * FROM history where id_username = :id_username", id_username = session["user_id"])

    return render_template("history.html", rows = rows)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        
        # ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("missing symbol")
        else:
            findsymbol = lookup(request.form.get("symbol"))
            if not findsymbol:
                return apology("invalid symbol")
                
            return render_template("quoted.html", name = findsymbol["name"], price = usd(findsymbol["price"]), symbol = findsymbol["symbol"])
    
    return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    
    if request.method == "POST":
        
        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        elif not request.form.get("password") == request.form.get("password2"):
            return apology("passwords don't match")
        
        else:
            result = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
            
            if not len(result) == 0:
                return apology("username taken")
            
            """
            Note: Because will meet below error, so I use hash() instead of encrypt().
            DeprecationWarning: the method passlib.context.CryptContext.encrypt() is deprecated as of Passlib 1.7, 
                and will be removed in Passlib 2.0, use CryptContext.hash() instead.
            """
            #hash = pwd_context.encrypt(request.form.get("password"))
            hash = pwd_context.hash(request.form.get("password"))
            #ok = pwd_context.verify(request.form.get("password"), hash)
            
            # Insert a new user.
            rows = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)", username = request.form.get("username"), hash = hash)
            
            # Let new user has logged in
            rows = db.execute("SELECT * FROM users WHERE username = :username", username = request.form.get("username"))
            session["user_id"] = rows[0]["id"]
            
            return redirect(url_for("index"))
    
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method == "POST":
        
    # ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("missing symbol")
        elif not request.form.get("shares"):
            return apology("missing shares")
        elif not request.form.get("shares").isdigit():
            return apology("invalid shares")
        elif int(request.form.get("shares")) < 1:
            return apology("invalid shares")
        else:
            # Check you have this symbol.
            rows = db.execute("SELECT symbol FROM history where id_username = :id_username group by symbol", id_username = session["user_id"])

            for c in range(len(rows)):
                if request.form.get("symbol").upper() == rows[c]["symbol"]:
                    break
                if c+1 == len(rows):
                    return apology("symbol not owned")
            
            # Check all shares more than you want sell.
            rows = db.execute("SELECT sum(shares) AS total_shares FROM history where id_username = :id_username and symbol = :symbol",\
                                id_username=session["user_id"],\
                                symbol=request.form.get("symbol").upper()
                                )
             
            if int(request.form.get("shares")) > rows[0]["total_shares"]:
                return apology("too many shares")
            
            findsymbol = lookup(request.form.get("symbol"))        
            rows = db.execute("INSERT INTO history (id_username, symbol, symbol_name, shares, price, transacted)\
                            VALUES(:id_username, :symbol, :symbol_name, :shares, :price, :transacted)",\
                            id_username =session["user_id"],\
                            symbol = findsymbol["symbol"],\
                            symbol_name = findsymbol["name"],\
                            shares = 0-int(request.form.get("shares")),\
                            price = "{:,.2f}".format(findsymbol["price"]),\
                            transacted = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                            )
                            
            rows = db.execute("UPDATE users SET cash = cash + :total where id = :id",\
                                total = findsymbol["price"] * int(request.form.get("shares")), \
                                id =session["user_id"]
                                )
        
            flash('Sold!')
            return redirect(url_for("index"))
        
    else:
        return render_template("sell.html")
        
@app.route("/changpwd", methods=["GET", "POST"])
@login_required
def changpwd():
    """Change the user password"""
    if request.method == "POST":
        
        # ensure password are correct
        if not request.form.get("password_cur"):
            return apology("must provide password")
        elif not request.form.get("password_new"):
            return apology("must provide new password")
        elif not request.form.get("password_chk"):
            return apology("must provide new password")
        elif not request.form.get("password_new") == request.form.get("password_chk"):
            return apology("passwords don't match")
        else:
            # Select user info.
            rows = db.execute("SELECT * FROM users WHERE id = :id", id =session["user_id"])

            # Check current password is correct.
            if not pwd_context.verify(request.form.get("password_cur"), rows[0]["hash"]):
                return apology("password not correct")
            
            # Update new password.
            rows = db.execute("UPDATE users SET hash = :hash where id = :id",\
                                hash = pwd_context.hash(request.form.get("password_new")),\
                                id = session["user_id"]
                                )
            
            # Log out, and re-log in your account with new password.
            session.clear()
            flash("Change Success! Please log in with your new password. Thanks!")
            return render_template("login_chgpwd.html")
    else:
        return render_template("changepwd.html")