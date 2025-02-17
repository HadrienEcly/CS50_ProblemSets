SELECT DISTINCT name FROM people
WHERE id IN (
    Select person_id FROM stars
    Where movie_id IN (
        SELECT movie_id FROM stars
        WHERE person_id = (SELECT id FROM people WHERE name = 'Kevin Bacon' AND birth = '1958'))
    AND person_id != (SELECT id FROM people WHERE name = 'Kevin Bacon' AND birth = '1958')
)
;
