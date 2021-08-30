# 242-a1

## Members

Madison Wong mwong
Shay Stevens sstevens
Chok Thonmeerit cthonmeerit

# Comparing Collisions Resolution Strategies

## With war-and-peace-20.txt (i.e. 18138 distinct words), default table size 113

The percent of keys placed at home is higher for linear probing when the
percent of the table full is less than 70%, but higher after then it evens
out between the two collision stratgies. The maximum number of collisions
starts out similar but as the table becomes fuller, there is a
lower using double hashing than linear probing. The average collisions is better
for linear probing when the table is less than 50% full (better meaning less),
and then becomes better using double hashing.

## Using war-and-peace-20.txt, table size 16000

Percent of words placed at home is very similar.
The maximum number of collisions starts out similar but as the
table becomes fuller it starts increasing quicker for linear probing than
for double hashing, which only balloons after 90% full. The average
collisions also stay similar until about 70% full where linear probing
increases average collisions more rapidly and balloons at 100% full, whereas
for double hashing at 100% full it did increase, but much less.

## Using war-and-peace-20.txt, table size 18500

The percent at home with the two collision strategies is very similar.
The maximum number of collisions also starts similar but starts increasing
faster and sooner with the linear probing than double hashing and we can see
the same pattern with the average collisions.

## Using war-and-peace-20.txt, table size 25000 (quite bigger than input size)

Percent at home is very similar. Average collisions also similar but
increasing faster for linear probing. Maximum collisions increases faster
with linear probing than double hashing.

## Summary

With these results, we can see that the percent of keys placed at home is
relatively similar between the two strategies. The maximum number of collisions
starts out similar when the table is emptier, but as it becomes fuller, it
increases quicker with linear probing than double hashing and the same case
follows for double hashing
