# SC1015-Project-Ay2021

## Description

* SC1015 (Project AY2021-22 Y1S2)
* [Jarrel](https://github.com/JarrelT), [Jing Kai](https://github.com/Useidinstead), [Vernon](https://github.com/Vernonlzy)

## Libraries

```
pip install numpy pandas seaborn
```

## About the data
* Data accquired from [Kaggle](https://www.kaggle.com) by [Sobhan Moosavi](https://www.kaggle.com/sobhanmoosavi)
* [US Accidents (2016 - 2021)](https://www.kaggle.com/datasets/sobhanmoosavi/us-accidents)
* [US Weather Events (2016 - 2021)](https://www.kaggle.com/datasets/sobhanmoosavi/us-weather-events)
```
US_Accidents_Dec21_updated.csv (202203212328, 1.1G)
WeatherEvents_Jan2016-Dec2021.csv (202203231900, 898M)
```
* Data are combined and analysed in city level (only a few cities)
* Cities are selected based on their geographical location (different part of the US)
* Denver, Miami, Minneapolis, New York & Phoneix
```
US_Accidents... + WeatherEvents... = [countries].csv
```
```
[countries].csv Columns: date,t1,t2,t3,t4,ts1,ts2,ts3,ts4,accS1,accS2,accS3,accS4,accT,accTF

date   : ranges from 2016/01/01 to 2021/12/31
t[x]   : represents weather type
ts[x]  : represents type severity
accS[x]: represents accident severity
accT   : represents total accidents in a day
accTF  : accident on that day ?
```
* Possible data fields (Same as original data set)
```
date   : string, date
t[x]   : string, ['Snow', 'Fog', 'Cold', 'Storm', 'Rain', 'Precipitation', 'Hail']
ts[x]  : string, ['Light', 'Severe', 'Moderate', 'Heavy', 'UNK', 'Other']
accS[x]: int
accT   : int
accTF  : string / boolean, TRUE | FALSE
```