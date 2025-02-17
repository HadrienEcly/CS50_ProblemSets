-- Keep a log of any SQL queries you execute as you solve the mystery.


-- Get description of the crime scene
SELECT description FROM crime_scene_reports
    WHERE street = 'Humphrey Street' AND year = '2024' AND month = '07' AND day = '28'
;
-- ANSWER = | Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery. | Littering took place at 16:36. No known witnesses.


-- Select name from licence plate that were still in the shop at the hour of the theft
SELECT name FROM people
WHERE license_plate IN (
SELECT license_plate
FROM bakery_security_logs
WHERE (year = 2024 AND month = 07 AND day = 28) AND (hour < 10 OR (hour = 10 AND minute < 16))
GROUP BY license_plate
HAVING SUM(CASE WHEN activity = 'entrance' THEN 1 ELSE 0 END) > SUM(CASE WHEN activity = 'exit' THEN 1 ELSE 0 END)
)
;
-- ANSWER : Vanessa, Barry, Brandon, Iman, Sofia, Taylor, Luca, Diana, Kelsey, Bruce, Sophia

-- Interview
SELECT * FROM interviews
WHERE (year = 2024 AND month = 07 AND day = 28)
;
-- HINT : Exit parking 10 minutes after the thieve | before Eugene arrived at the bakery, thief was withdrawing some money at Leggett Street | theif called smoneone <1 minute. Planning to take
-- earliest flight out of fiftyville tomorrow. Asked the person on the phone to buy a ticket

-- People wo exited the parking 10 minutes after the thief
SELECT * FROM people
WHERE license_plate IN (
SELECT license_plate
FROM bakery_security_logs
WHERE (year = 2024 AND month = 07 AND day = 28 AND hour = 10 AND (minute > 15 AND minute < 26)) AND activity = 'exit'
)
;
-- ANSWER : Vanessa, Barry, Iman, Sofia, Luca, Diana, Kelsey, Bruce

-- People withdrawing before Eugene arrived at the bakery
SELECT name FROM people
WHERE id IN (
    SELECT person_id FROM bank_accounts
    WHERE account_number IN (
        SELECT account_number FROM atm_transactions
        WHERE year = 2024 AND month = 07 AND day = 28 AND atm_location = 'Leggett Street'
    )
)
;
-- ANSWER : Kenny, Iman, Benista, Taylor, Brooke, Luca, Diana, Bruce, Kaelyn

-- Regrouping all the info we have a new list of suspect :
SELECT * FROM people
WHERE license_plate IN (
SELECT license_plate
FROM bakery_security_logs
WHERE (year = 2024 AND month = 07 AND day = 28 AND hour = 10 AND (minute > 15 AND minute < 26)) AND activity = 'exit'
) AND id IN (
    SELECT person_id FROM bank_accounts
    WHERE account_number IN (
        SELECT account_number FROM atm_transactions
        WHERE year = 2024 AND month = 07 AND day = 28 AND atm_location = 'Leggett Street'))
;
-- ANSWER : Iman, Luca, Diana, Bruce.

-- People who called for one than a minute on the day of the thief
SELECT name FROM people
WHERE phone_number IN(
SELECT caller FROM phone_calls
WHERE year = 2024 AND month = 07 AND day = 28 AND duration < 60);
-- ANSWER : Kenny, Sofia, Benista, Taylor, Diana, Kelsey, Bruce, Carina

-- new suspect list
SELECT * FROM people
WHERE license_plate IN (
SELECT license_plate
FROM bakery_security_logs
WHERE (year = 2024 AND month = 07 AND day = 28 AND hour = 10 AND (minute > 15 AND minute < 26)) AND activity = 'exit'
) AND id IN (
    SELECT person_id FROM bank_accounts
    WHERE account_number IN (
        SELECT account_number FROM atm_transactions
        WHERE year = 2024 AND month = 07 AND day = 28 AND atm_location = 'Leggett Street'))
            AND id IN (
                SELECT id FROM people
                WHERE phone_number IN(
                    SELECT caller FROM phone_calls
                    WHERE year = 2024 AND month = 07 AND day = 28 AND duration < 60))
;
-- ANSWER = Diana, Bruce

-- People that called the two suspect on this day during the limit duration
SELECT caller.name AS caller_name, receiver.name AS receiver_name
FROM phone_calls
JOIN people AS caller ON phone_calls.caller = caller.phone_number
JOIN people AS receiver ON phone_calls.receiver = receiver.phone_number
WHERE phone_calls.year = 2024 AND phone_calls.month = 07 AND phone_calls.day = 28 AND phone_calls.duration < 60;
-- ANSWER : Diana called Phillip and Bruce called Robin.

-- Need to see if Diana or BRuce is a passenger of the earliest flight out of fiftyville
SELECT name from people
WHERE passport_number IN (
    SELECT passport_number from passengers
    WHERE flight_id IN(
        SELECT id FROM flights
        WHERE year = 2024 AND month = 07 AND day = 29 AND origin_airport_id = (
            SELECT id FROM airports
            WHERE city = 'Fiftyville'
        )
        ORDER BY hour,minute
        LIMIT 1
    )
)
    ;
-- Answer : Bruce is in this flight.

-- I can already conclude that Bruce is the thief of the poor duck and his a Accomplice is Robin. Know I need to know their destination
SELECT city from airports
WHERE id = (
    SELECT destination_airport_id FROM flights
    WHERE year = 2024 AND month = 07 AND day = 29 AND origin_airport_id = (
        SELECT id FROM airports
        WHERE city = 'Fiftyville'
    )
    ORDER BY hour,minute
    LIMIT 1
)
;
-- ANSWER : New York City
