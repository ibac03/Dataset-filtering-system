# Tests for `SENG 265`, Assignment #3

* Pre-requisites
    * Configuration of required libraries (executed only once after connecting to Ref. Plat.): `setSENG265`
    * Compilation: `make`
    * Enable the execution of tester: `chmod u+x tester`

* Test 1
    * Input: `data.csv`
    * Expected output: `test01.csv`
    * Test: `./tester 1`
    * Command automated by tester: `./song_analyzer --data="data.csv" --filter="ARTIST" --value="Dua Lipa" --order_by="STREAMS" --order="ASC" --limit="6"`
    
* Test 2
    * Input: `data.csv`
    * Expected output: `test02.csv`
    * Test: `./tester 2`
    * Command automated by tester: `./song_analyzer --data="data.csv" --filter="ARTIST" --value="Drake" --order_by="STREAMS" --order="DES"`

* Test 3
    * Input: `data.csv`
    * Expected output: `test03.csv`
    * Test: `./tester 3`
    * Command automated by tester: `./song_analyzer --data="data.csv" --filter="YEAR" --value="2023" --order_by="NO_SPOTIFY_PLAYLISTS" --order="DES" --limit="5"`
    

* Test 4
    * Input: `data.csv`
    * Expected output: `test04.csv`
    * Test: `./tester 4`
    * Command automated by tester: `./song_analyzer --data="data.csv" --filter="YEAR" --value="2023" --order_by="NO_APPLE_PLAYLISTS" --order="DES" --limit="7"`
    