# 242-a1

## Members

Madison Wong mwong
Shay Stevens sstevens
Chok Thonmeerit cthonmeerit

# Comparing Collisions Resolution Strategies

## With war-and-peace-20.txt (i.e. 18138 distinct words), default table size 113

Linear Probing
Percent   Current    Percent    Average      Maximum
Full     Entries    At Home   Collisions   Collisions
------------------------------------------------------
10         11        90.9       0.09           1
20         22        86.4       0.14           1
30         33        75.8       0.36           4
40         45        71.1       0.40           4
50         56        69.6       0.46           4
60         67        65.7       0.60           4
70         79        57.0       1.20          14
80         90        53.3       1.68          17
90        101        49.5       3.03          42
100        113        46.0       6.60         107
------------------------------------------------------

Double Hashing
Percent   Current    Percent    Average      Maximum
Full     Entries    At Home   Collisions   Collisions
------------------------------------------------------
10         11        81.8       0.18           1
20         22        81.8       0.18           1
30         33        69.7       0.36           3
40         45        66.7       0.42           3
50         56        62.5       0.45           3
60         67        62.7       0.49           3
70         79        57.0       0.67           4
80         90        53.3       0.82           9
90        101        49.5       1.12           9
100        113        46.0       2.99          58
------------------------------------------------------

It can be seen that at the default table size, which is much less than the
input size, that the percentage of keys placed at home is higher for
linear probing when the percent of the table full is less than 70%, but
higher than that it evens out between the two collision stratgies.

The maximum number of collisions starts out relatively the same for two
stratgies but as the table becomes fuller (around 70%), there is a
significantly lower maximum number of collisions using double hashing than
linear probing.

The average collisions is better for linear probing when the table is less
than 50% full (better meaning less), and then becomes better using double
hashing.

## Using war-and-peace-20.txt (i.e. 18138 distinct words), table size 16000
## i.e. more similar to input size but still smaller

Linear Probing
Percent   Current    Percent    Average      Maximum
Full     Entries    At Home   Collisions   Collisions
------------------------------------------------------
10       1600        94.6       0.06           4
20       3200        90.1       0.12           6
30       4800        85.5       0.21           9
40       6400        80.3       0.35          16
50       8000        74.6       0.56          37
60       9600        69.8       0.86          63
70      11200        64.8       1.39         144
80      12800        59.7       2.64         262
90      14400        54.7       8.11        1044
100      16001        49.6     125.28       15599
------------------------------------------------------

Double Hashing
Percent   Current    Percent    Average      Maximum
Full     Entries    At Home   Collisions   Collisions
------------------------------------------------------
10       1600        94.5       0.06           3
20       3200        89.9       0.12           4
30       4800        85.7       0.18           5
40       6400        80.9       0.27           7
50       8000        75.5       0.39          10
60       9600        70.1       0.53          12
70      11200        65.1       0.72          18
80      12800        60.1       1.03          34
90      14400        55.0       1.59          47
100      16001        50.0       8.91       13285
------------------------------------------------------

It can be seen that with this table size which is much closer to the number of
distinct words, that the percent of words placed at home is very similar for
the different collision resolution stratgies.

The maximum number of collisions starts out relatviely the same for two
stratgies but as the table becomes fuller it starts increasing quicker for
linear probing than for double hashing which only balloons after 90% full.

The average collisions also stay relatively similar until about 70% full where
linear probing increases average collisions more rapidly than double hashing
and balloons at 100% full, whereas for double hashing at 100% full it did
increase, but much less than with linear probing.

proportion of keys placed in home cell i.e. not colliding
maximum number of collisions before being palced into table
average collisions before hash table placement 


## Linear Probing



## Double Hashing 

