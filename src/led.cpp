// ws bits format

/*
format | data
00 |  00 ff 00 ff ...

Format 00: (individual mode)
     led 0         led 1    led 2    ...
00 |  11 22 33 | 11 22 33 | 11 22 33 ...
      r  g  b    r  g  b    r  g  b  ...


Format 01: (fill all mode)
01 |  ff ff ff 
      r  g  b


*/