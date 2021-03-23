## Pisah

Pisah (translates to "separate" in Malay) is a tool for splitting a natural language document into sentences.
Currently Pisah supports only English.

The library is mostly based on:
    * Pragmatic Sentence Segmenter (Ruby) (https://github.com/diasks2/pragmatic_segmenter)
    * PySBD (Python): (https://github.com/nipunsadvilkar/pySBD/)

#### Installation
* Install PCRE library. Run `sudo apt-get install libpcre3 libpcre3-dev` on Ubuntu machines.

* Run the following commands
```
mkdir build
cd build
cmake ..
make -j
```

* To test:
```
echo "How are you, Mr. John? It has been so long since we talked." | ./pisah 

```
