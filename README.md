# PISAH
Pisah (translates to "separate" in Malay) is a tool for splitting a natural language document into sentences.
Currently Pisah supports only English.

The library is mostly based on:

    * Pragmatic Sentence Segmenter (Ruby) (https://github.com/diasks2/pragmatic_segmenter)
    * PySBD (Python): (https://github.com/nipunsadvilkar/pySBD/)

#### Installation of PCRE library
* UBUNTU: Run `sudo apt-get install libpcre3 libpcre3-dev`
* MacOSX: Run `brew install pcre`

#### BUILD
  
```
mkdir build && cd build
cmake ..
make -j
```

#### TEST
```
echo "How are you, Mr. John? It has been so long since we talked." | ./pisah 

```
