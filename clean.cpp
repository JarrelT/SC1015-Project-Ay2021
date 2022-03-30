#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <ctime>

using namespace std;

#define _VERSION_ "Subsidiary program v1.00 202203300909"
#define _BIRTH___ "202203231855"

typedef struct {
	string weather[4];
	string weatherS[4];
	int accS[4];
	int accT;
} row_t;

char* isInt(const char* b_);

time_t strTime(char* b_, struct tm* default_);

string getCol(const char* buf, int idx);

void initRows(row_t* r, size_t n);

int main(int argc, char* argv[])
{
	/* compile this source and run the program with country (1 at a time) as argument */
	/* eg. ./[prog] 'New York' */
	const size_t IDX = 2200;

	ifstream weather("WeatherEvents_Jan2016-Dec2021.csv"); // File name change accordingly
	ifstream acc("US_Accidents_Dec21_updated.csv");
	ofstream of(string((argc >= 2) ? argv[1] : "Miami") + ".csv");

	const time_t ST = 1451577600; /* 20160101000000 */
	const time_t ET = 1640880000; /* 20211231000000 */
	const time_t RT = 3600 * 24; /* Time resolution is a day */
	row_t* rows = new row_t[IDX]; /* 6 years ~ 2190 days */

	string buf;
	int colCount = 0;

	struct tm tmp_tm = { 0 };
	/*
		
		TODO: add percipitation columns

		total 2016 01 30 to 2021 12 31, 6 years ~ 2190 days
		date,t1,t2,t3,t4,ts1,ts2,ts3,ts4,accS1,accS2,accS3,accS4,accT,accTF
		UTC TIME IS USED FOR WEATHER
		WHILE LOCAL TIME IS USED FOR ACCIDENTS
	*/
	time_t CTZ = 3600 * 4, row_time = ST; /* GMT-4 for Miami [UPDATE AS REQUIRED] */
	const string city = (argc >= 2) ? argv[1] : "Miami"; /* [UPDATE AS REQUIRED] */

	if (!weather || !acc || !of) {
		cerr << "File not found or permission denied for I/O operation\n";
		return 1;
	}
	initRows(rows, IDX);

	getline(weather, buf);
	cout << "Processing: " << getCol(buf.c_str(), 10) << "\n";
	while (getline(weather, buf)) {
		if (getCol(buf.c_str(), 10) == city) {
			string type = getCol(buf.c_str(), 1); /* 'Snow', 'Fog', 'Cold', 'Storm', 'Rain', 'Precipitation', 'Hail' */
			string severity = getCol(buf.c_str(), 2); /* 'Light', 'Severe', 'Moderate', 'Heavy', 'UNK', 'Other' */
			string startTime = getCol(buf.c_str(), 3);
			string endTime = getCol(buf.c_str(), 4);
			time_t st = strTime((char*)startTime.c_str(), &tmp_tm); /* Given UTC TIME */
			time_t et = strTime((char*)endTime.c_str(), &tmp_tm); /* Given UTC TIME */
			size_t row_idx = (size_t)((double)(st - ST) / RT);
			size_t col_idx_s = ((st - ST) % RT) / 21600;
			size_t col_idx_e = ((et - ST) % RT) / 21600;
			rows[row_idx].weather[col_idx_s] = type;
			rows[row_idx].weather[col_idx_e] = type;
			rows[row_idx].weatherS[col_idx_s] = severity;
			rows[row_idx].weatherS[col_idx_e] = severity;
			colCount++;
			if (buf.find("2021-12-31") != string::npos)
				cout << row_idx << ":" << col_idx_s << "\t" << severity << "\t" << startTime << endl;
		}
	}
	cout << "Found " << colCount << " entries for " << city << " (weather)" << endl;

	getline(acc, buf);
	colCount = 0;
	while (getline(acc, buf)) {
		if (getCol(buf.c_str(), 13) == city) {
			string severity = getCol(buf.c_str(), 1); /* 4, 3, 2, 1 */
			string startTime = getCol(buf.c_str(), 2); /* Given local time */
			time_t st = strTime((char*)startTime.c_str(), &tmp_tm) + CTZ; /* UTC */
			size_t row_idx = (size_t)((double)(st - ST) / RT);
			size_t col_idx_s = ((st - ST) % RT) / 21600;
			rows[row_idx].accS[col_idx_s] = atoi(severity.c_str());
			rows[row_idx].accT++;
			colCount++;
			if (buf.find("2021-12-31") != string::npos)
				cout << row_idx << ":" << col_idx_s << "\t" << severity << "\t" << startTime << endl;
		}
	}
	cout << "Found " << colCount << " entries for " << city << " (accidients)" << endl;
	weather.close();
	acc.close();

	of << "Date,W0000_0600,W0601_1200,W1201_1800,W1801_2359,WS1,WS2,WS3,WS4,";
	of << "A0000_0600,A0601_1200,A1201_1800,A1801_2359,A_Total,Accident\n";
	for (size_t i = 0; i < IDX || row_time <= ET; i++) {
		struct tm* tm_ = localtime(&row_time);
		of << setfill('0') << setw(4) << tm_->tm_year + 1900 << "-";
		of << setfill('0') << setw(2) << tm_->tm_mon + 1 << "-";
		of << setfill('0') << setw(2) << tm_->tm_mday << ",";
		for (size_t j = 0; j < 4; j++)
			of << rows[i].weather[j] << ",";
		for (size_t j = 0; j < 4; j++)
			of << rows[i].weatherS[j] << ",";
		for (size_t j = 0; j < 4; j++)
			of << rows[i].accS[j] << ",";
		of << rows[i].accT << "," << ((rows[i].accT) ? "TRUE\n" : "FALSE\n");
		row_time += RT;
	}

	return 0;
}
void initRows(row_t* r, size_t n)
{
	for (size_t i = 0; i < n; i++) {
		r[i].accT = 0;
		r[i].weather[0] = "Clear";
		r[i].weather[1] = "Clear";
		r[i].weather[2] = "Clear";
		r[i].weather[3] = "Clear";
		r[i].weatherS[0] = "NA";
		r[i].weatherS[1] = "NA";
		r[i].weatherS[2] = "NA";
		r[i].weatherS[3] = "NA";
		r[i].accS[0] = 0;
		r[i].accS[1] = 0;
		r[i].accS[2] = 0;
		r[i].accS[3] = 0;
	}
}

string getCol(const char* buf, int idx)
{
	int s = 0;
	char *cur = (char*)buf, *prev = (char*)buf;
	while ((cur = strchr(cur, ','))) {
		if (s == idx)
			return string(prev, cur - prev);
		s++;
		prev = ++cur;
	}
	// last cell will not be ignored
	return "";
}

char* isInt(const char* b_)
{
	for (size_t i = 0; b_[i] != '\0'; i++)
		if (!(b_[i] >= '0' && b_[i] <= '9'))
			return nullptr;
	return (b_[0] == '\0') ? nullptr : (char*)b_;
}

time_t strTime(char* b_, struct tm* default_)
{
	/*
		Basically this converts char* to time_t
		YYYY[/|-]MM[/|-]DD hh[:|.]mm[:|.]ss hh[:|.]mm
		I agree that this can be alot shorter, C compatible.
		default_ will be used if any of the fields are not given
		Eg. Given YYYY/MM, the DD = default_->tm_mday
	*/
	char* sep_d = "/-";
	char* sep_h = ":.";
	char* token[512];

	time_t result;
	struct tm indv;

	size_t count = 0;
	size_t sc = 2;

	char* p = b_;

	int* ref[] = {
		&(indv.tm_year),
		&(indv.tm_mon),
		&(indv.tm_mday)
	};

	indv.tm_year = -1;
	indv.tm_mon = -1;
	indv.tm_mday = -1;
	indv.tm_hour = -1;
	indv.tm_min = -1;
	indv.tm_sec = -1;
	indv.tm_isdst = -1;

	token[count++] = b_;

	while ((p = strchr(p, ' '))) {
		*p = '\0';
		token[count++] = ++p;
	}
	// 2021[/-]12[/-]25
	for (size_t i = 0; i < count; i++) {

		if (token[i][0] == '\0')
			continue;

		char* f;
		size_t s_idx = -1;

		for (size_t j = 0; j < sc; j++) {
			f = token[i];
			if ((f = strchr(f, sep_d[j]))) {
				s_idx = j;
				break;
			}
		}

		if (!f)
			continue;

		char* s = strchr(f + 1, sep_d[s_idx]);
		int n[3];

		if (!s)
			continue;

		*f = '\0', *s = '\0';
		if (isInt(token[i]) && isInt(f + 1) && isInt(s + 1)) {
			n[0] = atoi(token[i]);
			n[1] = atoi(f + 1);
			n[2] = atoi(s + 1);
			indv.tm_year = -1;
			indv.tm_mon = -1;
			indv.tm_mday = -1;
		} else {
			*f = sep_d[s_idx];
			*s = sep_d[s_idx];
			continue;
		}
		for (size_t k = 0; k < 3; k++) {
			if (n[k] > 31) {
				indv.tm_year = n[k] - 1900;
				n[k] = -1;
			} else if (n[k] > 12) {
				indv.tm_mday = n[k];
				n[k] = -1;
			}
		}
		for (size_t k = 0; k < 3; k++) {
			if (*(ref[k]) == -1)
				for (size_t m = 0; m < 3; m++)
					if (n[m] != -1) {
						*(ref[k]) = n[m];
						n[m] = -1;
						break;
					}
		}
		indv.tm_mon--;
		*f = sep_d[s_idx];
		*s = sep_d[s_idx];
		break;
	}
hms:
	// hh[:.]mm[:.]ss hh[:.]mm
	for (size_t i = 0; i < count; i++) {

		if (token[i][0] == '\0')
			continue;

		char* f;
		size_t s_idx = -1;

		for (size_t j = 0; j < sc; j++) {
			f = token[i];
			if ((f = strchr(f, sep_h[j]))) {
				s_idx = j;
				break;
			}
		}

		if (!f)
			continue;

		char* s = strchr(f + 1, sep_h[s_idx]);
		int n[3];

		*f = '\0';
		if (s)
			*s = '\0';

		if (s && isInt(token[i]) && isInt(f + 1) && isInt(s + 1)) {
			indv.tm_hour = atoi(token[i]);
			indv.tm_min = atoi(f + 1);
			indv.tm_sec = atoi(s + 1);
		} else if (!s && isInt(token[i]) && isInt(f + 1)) {
			indv.tm_hour = atoi(token[i]);
			indv.tm_min = atoi(f + 1);
			indv.tm_sec = 0;
		} else {
			*f = sep_h[s_idx];
			if (s)
				*s = sep_h[s_idx];
			continue;
		}
		*f = sep_h[s_idx];
		if (s)
			*s = sep_h[s_idx];
		break;
	}
	// undo delimit
	for (size_t i = 1; i < count; i++) {
		*(--token[i]) = ' ';
	}

	indv.tm_year = (indv.tm_year == -1) ? default_->tm_year : indv.tm_year;
	indv.tm_mon = (indv.tm_mon == -1) ? default_->tm_mon : indv.tm_mon;
	indv.tm_mday = (indv.tm_mday == -1) ? default_->tm_mday : indv.tm_mday;
	indv.tm_hour = (indv.tm_hour == -1) ? 0 : indv.tm_hour;
	indv.tm_min = (indv.tm_min == -1) ? 0 : indv.tm_min;
	indv.tm_sec = (indv.tm_sec == -1) ? 0 : indv.tm_sec;

	return mktime(&indv);
}