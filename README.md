# RCNVMTrace

### Basic Usage
To make this project
```
$ make
```

To generate RCNVM trace
```
$ ./gen_trace.exe h
```

To generate DRAM trace
```
$ ./gen_trace.exe d
```


### OLXP workloads for RC-NVM evaluation

| Benchmark # | SQL Statement                                                                                                         | Description                                                                                                                   |
|-------------|-----------------------------------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------|
| Q1          | SELECT f3, f4 FROM table-a WHERE f10 > x                                                                              | Read f3 and f4 fields of certain tuples                                                                                       |
| Q2          | SELECT * FROM table-a1 WHERE f10 > x                                                                                  | Display certain rows that meet the condition; only a few rows that meet the restriction are selected                          |
| Q3          | SELECT * FROM table-a2 WHERE f10 > x                                                                                  | Same as Q2. Here most of f10 is greater than x, thus many rows are selected                                                   |
| Q4          | SELECT SUM(f9) FROM table-a1 WHERE f10 > x                                                                            | Show sum of f9 fields that meet the condition; only a few items meet the restriction; f9 and f10 locate in the same cacheline |
| Q5          | SELECT SUM(f9) FROM table-a2 WHERE f10 > x                                                                            | Same as Q4. Here most of f10 is greater than x, thus many rows are selected; f9 and f10 locate in the same cacheline          |
| Q6          | SELECT AVG(f1) FROM table-a1 WHERE f10 > x                                                                            | Show average value of f1 fields that meet the condition; only a few items meet the restriction                                |
| Q7          | SELECT AVG(f1) FROM table-a2 WHERE f10 > x                                                                            | Same as Q6. Here most of f10 is greater than x, thus many rows are selected                                                   |
| Q8          | SELECT table-a1.f3, table-a2.f4 FROM table-a1, table-a2 WHERE table-a1.f1 > table-a2.f1 AND table-a1.f9 = table-a2.f9 | Join example using 2 tables, in which both restrictions must be met                                                           |
| Q9          | SELECT table-a1.f3, table-a2.f4 FROM table-a1, table-a2 WHERE table-a1.f9 = table-a2.f9                               | Another join example with 2 tables, in which there is only one restriction                                                    |
| Q10         | SELECT f3, f4 FROM table-a WHERE f1 > x AND f9 < y                                                                    | Read f3 and f4 fields in which both restrictions must be met; f1 and f9 are not in the same cacheline                         |
| Q11         | SELECT f3, f4 FROM table-a WHERE f1 > x AND f2 < y                                                                    | Read f3 and f4 fields in which both restrictions must be met; f1 and f2 locate in the same cacheline                          |
| Q12         | UPDATE table-a SET f3 = x, f4 = y WHERE f10 = z                                                                       | Update f3 and f4 of certain rows that meet the condition                                                                      |
| Q13         | UPDATE table-a SET f9 = x WHERE f10 = y                                                                               | Update f9 of certain rows that meet the condition                                                                             |
| Q14         | SELECT SUM(f2_wide) FROM table-b                                                                                      | An OLAP query to read wide field f2_wide                                                                                      |
| Q15         | SELECT f3, f6, f10 FROM table-a                                                                                       | A query to read multiple fields                                                                                               |










