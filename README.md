# SC1015-Project-Ay2021

## Description

* SC1015 (Project)

## About the data
* Main data accquired from [Kaggle](https://www.kaggle.com/datasets/sobhanmoosavi/us-accidents)
```
US_Accidents_Dec21_updated.csv (202203212328, 1.1G)
```
* Due to the large data size, five states were filtered out for analysis:
```
grep -E ',Description,|,FL,|,OH,|,NY,|,VT,|,CA,' US_Accidents_Dec21_updated.csv > data.csv

[CA]	California
[FL]	Florida
[OH]	Ohio
[NY]	New York
[VT]	Vermont
```
