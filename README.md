# 242-a1

## Members

Madison Wong mwong
Shay Stevens sstevens
Chok Thonmeerit cthonmeerit

# Comparing Collisions Resolution Strategies

#### Input 18138 distinct words (war-and-peace-20.txt), table size 113
##### i.e. table size significantly smaller than input size 

The percent of keys placed at home is higher for linear probing when the
percent of the table full is less than 70%, but higher after then it evens
out between the two collision strategies. The maximum number of collisions
starts out similar but as the table becomes fuller, it is lower using double
hashing than linear probing. The average collisions are better for linear
probing when the table is less than 50% full (better meaning less),
and then becomes better using double hashing.

#### Input 18138 distinct words, table size 16000
##### i.e. table size close but smaller than input size

Percent of words placed at home is very similar. The maximum number of
collisions start out similar but as the table becomes fuller it starts
increasing quicker for linear probing than for double hashing, which only
balloons after 90% full. The average collisions also stay similar until about
70% full where linear probing increases average collisions more rapidly and
balloons at 100% full, whereas for double hashing at 100% full it did increase,
but much less.

#### Input 18138 distinct words, table size 18500
##### i.e. table size close and bigger than input size 

The percent at home with the two collision strategies is very similar.
The maximum number of collisions also starts similar but starts increasing
faster and sooner with the linear probing than double hashing and we can see
the same pattern with the average collisions.

#### Input 18138 distinct words, table size 25000
##### i.e. table size quite larger than input size 

Percent at home is very similar. Average collisions also similar but
increasing faster for linear probing. Maximum collisions increase faster
with linear probing than double hashing.

#### Input 100 distinct words (modified war-and-peace-20.txt)

We ran the same tests using only 100 distinct words compared to the
war-and-peace-20.txt 18138 distinct words with table sizes significantly
smaller than input, close but smaller than input, close and bigger than input,
and significantly bigger than input and arrived at the same conclusions.

## Summary

With these results, we can see that the percent of keys placed at home is
relatively similar between the two strategies. The maximum number of collisions
starts out similar when the table is emptier, but as it becomes fuller, it
increases quicker with linear probing than double hashing and the same case
follows for double hashing.

Therefore, if we believe that the input will not fill up much of the hash table,
it will be okay to use either linear probing or double hashing as they have
similar statistics in terms of percent at home, average collisions, and maximum
collisions. But, if the input is going to fill up the majority of the table,
double hashing will be better. And therefore if we are not sure how full the
hash table will be, it would be better to use double hashing as the collision
resolution strategy.
