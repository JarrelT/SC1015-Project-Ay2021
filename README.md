# SC1015-Project-Ay2021

## Description

* SC1015 (Project AY2021-22 Y1S2)
* [Jarrel](https://github.com/JarrelT), [Jing Kai](https://github.com/Useidinstead), [Vernon](https://github.com/Vernonlzy)

## About the data
* Data accquired from [Kaggle](https://www.kaggle.com/datasets/sobhanmoosavi/us-accidents)
* US Accidents (2016 - 2021) by [Sobhan Moosavi](https://www.kaggle.com/sobhanmoosavi)
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
## Libraries

```
pip install numpy pandas seaborn
```