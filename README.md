# roguelike

to-do list:
- [ ] generation of control file (not now please)
- [ ] working makefile
- [x] monster interactions (2022/06/03)
- [x] enemies... (it is alive!) (2022/04/03 + 2022/05/05)
- [x] cofiguration file (2022/03/26)
- [x] more sophiscated map scroll (2022/03/25)
- [x] map scroll (2022/03/25)

magic scheme:   
F - FIRE  
W - WATER  
E - EARTH  
S - SOUL  
B - BODY  

```-----------------------------------------------
  ↙   |   F   |   W   |   E   |   S   |   B   |
-----------------------------------------------
FIRE  |  x0.5 |  x0.5 |    x2 |    x1 |    x1 |
-----------------------------------------------
WATER |    x2 |  x0.5 |  x0.5 |    x1 |    x1 |
-----------------------------------------------
EARTH |  x0.5 |    x2 |  x0.5 |    x1 |    x1 |
-----------------------------------------------
SOUL  |    x1 |    x1 |    x1 |  x1.5 |  x0.5 |
-----------------------------------------------
BODY  |    x1 |    x1 |    x1 |  x0.5 |  x1.5 |
-----------------------------------------------
```
