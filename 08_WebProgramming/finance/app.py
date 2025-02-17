import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd, is_numerical, is_integer
from datetime import datetime
# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():

    # Show all stocks

    users = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
    stocks = db.execute("SELECT * FROM stocks WHERE user_id = ?", session["user_id"])
    stock_data = []
    total_cash = float(users[0]['cash'])

    # Store new data from server (like the price the name and the total)
    for stock in stocks:
        data = lookup(stock["what"])
        stock_data.append({
            "name": data["name"],
            "amount": int(stock["amount"]),
            "price": data["price"],
            "usdprice": usd(data["price"]),
            "total": float(data["price"]) * int(stock["amount"]),
            "usdtotal": usd(float(data["price"]) * int(stock["amount"]))
        })

    # Store total cash of the user to pass i through the html
    for stock in stock_data:
        total_cash += float(stock['total'])

    return render_template("index.html", user_cash=usd(float(users[0]['cash'])), stocks=stocks, stock_data=stock_data, total_cash=usd(total_cash))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():

    now = datetime.now()

    """Buy shares of stock"""
    if request.method == "POST":

        stock_tobuy = lookup(request.form.get('symbol'))

        # Check symbol input
        if not stock_tobuy:
            return apology("invalid stock symbol", 400)

        user_amount = float(db.execute("SELECT cash FROM users WHERE id = ?",
                            session["user_id"])[0]["cash"])
        shares_amount = request.form.get('shares')

        # Check shares input is a valid number
        if not is_integer(shares_amount):
            return apology("invalid value of shares", 400)

        shares_amount = int(shares_amount)
        # check if not negative
        if shares_amount < 1:
            return apology("invalid amount of shares", 400)

        total_price = float(stock_tobuy['price'])*float(shares_amount)
        if total_price > user_amount:
            return apology("User don't have enough money", 400)

        # update the tables

        # Add the stocks
        if not db.execute("SELECT * FROM stocks WHERE user_id=? AND what =?", session["user_id"], stock_tobuy["symbol"]):
            # Add the stock to the database
            db.execute("INSERT INTO stocks (user_id, what, amount) VALUES (?, ?, ?)",
                       session["user_id"], stock_tobuy["symbol"], shares_amount)
        else:
            # Increment stock count
            db.execute("UPDATE stocks SET amount = amount + ?", shares_amount)

        # Add the transaction
        db.execute("INSERT INTO transactions (user_id, what, amount, type, price, minutes, hour, day, month, year) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)",
                   session["user_id"], stock_tobuy["symbol"], shares_amount, 'buy', total_price, now.minute, now.hour, now.day, now.month, now.year)

        # Remove the money
        db.execute("UPDATE users SET cash = ? WHERE id = ?",
                   user_amount-total_price, session["user_id"])

        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    users = db.execute("SELECT * FROM users WHERE id = ? ", session["user_id"])
    transactions = db.execute(
        "SELECT * FROM transactions WHERE user_id = ? ORDER BY id DESC", session["user_id"])
    for transaction in transactions:
        transaction['price'] = usd(float(transaction['price']))
    return render_template("history.html", users=users, transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":

        lookedup_quote = lookup(request.form.get("symbol"))

        # Didn't used a variable for register for security issues, here it is not a problem
        if lookedup_quote == None:
            return apology("symbol doesn't exist", 400)

        return render_template("quote.html", name=lookedup_quote['name'], price=usd(lookedup_quote['price']), symbol=lookedup_quote['symbol'])

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":

        # Ensure username/password/confirmation are valid
        if not request.form.get("username"):
            return apology("must provide username", 400)

        for usernames in db.execute("SELECT username FROM users"):
            if request.form.get("username") == usernames['username']:
                return apology("username already used", 400)

        if not request.form.get("password"):
            return apology("must provide password", 400)

        if request.form.get("password") != request.form.get("confirmation"):
            return apology("password doesn't match confirmation", 400)

        # Add username and password to db
        username = request.form.get("username")
        password_hashed = generate_password_hash(request.form.get("password"))
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, password_hashed)

        session["user_id"] = db.execute(
            "SELECT id FROM users WHERE username == ?", request.form.get("username"))[0]['id']

        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    now = datetime.now()

    user = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]
    user_amount = user["cash"]
    user_actual_stocks = db.execute(
        "SELECT what,amount FROM stocks WHERE user_id = ?", session["user_id"])
    stock_list = []

    for stock in user_actual_stocks:
        stock_list.append(stock['what'])

    if request.method == "POST":
        stock_tosell = lookup(request.form.get("symbol"))
        shares_amount = request.form.get("shares")

        # check symbol input
        if not stock_tosell:
            return apology("symbol incorrect", 400)
        if stock_tosell['symbol'] not in stock_list:
            return apology("User don't have any shares of this stock", 400)

        # check shares input
        if not shares_amount:
            return apology("invalid amount of shares", 400)
        if not is_integer(shares_amount):
            return apology("invalid value of shares", 400)
        shares_amount = int(shares_amount)
        if shares_amount < 1:
            return apology("Can't sell 0 or less shares", 400)

        user_shares_count = db.execute(
            "SELECT amount FROM stocks WHERE user_id = ? AND what = ?", session["user_id"], stock_tosell["symbol"])
        shares_amount = int(shares_amount)
        total_price = float(stock_tosell['price'])*float(shares_amount)
        user_total = user_amount + total_price

        # Check if enough shares
        if shares_amount > user_shares_count[0]['amount']:
            return apology("Not enough shares", 400)

        new_share_count = user_shares_count[0]['amount'] - shares_amount
        # Update shares amount if shares in db = 0 delete row
        if new_share_count == 0:
            db.execute("DELETE FROM stocks WHERE user_id = ? AND what = ?",
                       session["user_id"], request.form.get("symbol"))
        else:
            db.execute("UPDATE stocks SET amount = ? WHERE user_id = ? AND what = ?",
                       new_share_count, session["user_id"], request.form.get("symbol"))

        # Update cash in db
        db.execute("UPDATE users SET cash = ? WHERE id = ?", user_total, session["user_id"])

        # Update transactions in db
        db.execute("INSERT INTO transactions (user_id, what, amount, type, price, minutes, hour, day, month, year) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)",
                   session["user_id"], stock_tosell["symbol"], shares_amount, 'sell', total_price, now.minute, now.hour, now.day, now.month, now.year)

        return redirect("/")

    else:
        return render_template("sell.html", user_actual_stocks=user_actual_stocks)


@app.route("/add", methods=["GET", "POST"])
@login_required
def add():
    if request.method == "POST":
        cash = request.form.get("cash")
        user = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]
        user_amount = float(user["cash"])

        # Check if input is correct
        if not cash or not is_numerical(cash):
            return apology("Cash not valid", 403)

        # Add cash to database
        new_total = user_amount + float(cash)
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_total, session["user_id"])

        return redirect("/")
    return render_template("add.html")
