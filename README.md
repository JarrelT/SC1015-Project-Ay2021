# SC1015-Project-Ay2021

## Description

* SC1015 (Project AY2021-22 Y1S2)
* Repository for our group’s mini project for SC-1015 investigating the possibility of weather affecting the rate of traffic accidents occurring.
* Done By: [Jarrel](https://github.com/JarrelT), [Jing Kai](https://github.com/Useidinstead), [Vernon](https://github.com/Vernonlzy)
![Screenshot 2022-04-24 002857](https://user-images.githubusercontent.com/92446140/164914848-d00cae9f-5eeb-4eaf-a9e5-c2172604324b.png)


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
[countries].csv Columns: Date,W1,W2,W3,W4,WS1,WS2,WS3,WS4,AS1,AS2,AS3,AS4,P_Value(in),A_Total,Accident

date     : ranges from 2016/01/01 to 2021/12/31
W[x]     : represents weather type
WS[x]    : represents weather severity
AS[x]    : represents accident severity
A_Total  : represents total accidents in a day
Accident : accident on that day ?
```
* Possible data fields (Same as original data set)
```
date     : string, date
W[x]     : string, ['Snow', 'Fog', 'Cold', 'Storm', 'Rain', 'Precipitation', 'Hail']
WS[x]    : string, ['Light', 'Severe', 'Moderate', 'Heavy', 'UNK', 'Other']
AS[x]    : int
A_Total  : int
Accident : boolean, TRUE | FALSE
```
