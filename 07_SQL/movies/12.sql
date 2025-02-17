SELECT DISTINCT title
FROM movies
JOIN stars AS sBrad ON movies.id = sBrad.movie_id
JOIN stars AS sJen ON movies.id = sJen.movie_id
WHERE sBrad.person_id = (
    SELECT id FROM people WHERE name = 'Bradley Cooper')
    AND sJen.person_id = (
        SELECT id FROM people WHERE name = 'Jennifer Lawrence'
    )
;
