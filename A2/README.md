- [Assignment PDF](./A2.pdf)
- [Agent Implementation Script](./rollerball/src/engine.cpp)
- [Include Header File for Agent Implementation](./rollerball/src/engine.hpp)
- [Makefile](./rollerball/Makefile)

Launching GUI:
```
cd rollerball/web
python3 -m http . server 8080
```
Launching the Bot:
```
cd rollerball
make rollerball
./bin/rollerball -p 8181
```
