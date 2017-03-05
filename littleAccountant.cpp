#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;
string months[12] = { "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

int dateSTOI(string date){
	string month = date.substr(0,3);//takes three letters that hold the month
	int day = stoi(date.substr(4,5)); //assume all date are valid
	for (int toCheck = 1; toCheck <= 12; toCheck++){
		if (month == months[toCheck-1]) return (toCheck*100 + day);
	}
	return -2;
}

string dateITS(int date){
	string date_string = months[(date/100) - 1];
	date_string += '.';
	date = date % 100;
	date_string += ('0' + date/10);
	date_string += ('0' + date%10);
	return date_string;
}
struct accountSummary{
	string description;
	string accountID;
	int debit;
	int credit;
};

class account
{
	vector<bool> debit_credit;//returns true for credit, false for debit of transaction at point i
	vector<int> amount;// return the amount references in i
	vector<int> date;//the date as an int in the form MMDD for easier comparison later on
	string description;
	string accountNum;

public:

	void addTransaction(string date_string, string credit, string debit){ // will add the transaction to the clients account
		int intCredit = stoi(credit);
		int intDebit = stoi(debit);
		debit_credit.push_back(intCredit);// if intCredit != 0 it will be saved as true, and if it is zero then saved as false
		int trans_amount = ((intCredit) ? intCredit : intDebit);
		amount.push_back(trans_amount);
		int dateInt = dateSTOI(date_string);
		date.push_back(dateInt);
		return;
	}

	void updateDescriptionID(string newDescription, string ID){
		description = newDescription;
		accountNum = ID;
		return;
	}

	accountSummary lookInTheBooks(string startDate, string endDate){
		int checkStart = (startDate == "*") ? 101 : dateSTOI(startDate);
		int checkEnd = (endDate == "*") ? 1231 : dateSTOI(endDate);
		int runningDebit = 0;
		int runningCredit = 0;
		for (int i = 0; i < date.size(); i++){
			if (checkEnd < date[i]) break;
			if (date[i] >= checkStart){
				if (debit_credit[i]) runningCredit += amount[i];
				else runningDebit += amount[i];
			}
		}
		if (date.size() < 1) runningDebit = -1;
		accountSummary checkedRecords = { description, accountNum, runningDebit, runningCredit};
		return checkedRecords;
	}

	void printTheBook(){
		cout << "ACCOUNT NUMBER: \t" << accountNum << endl;
		cout << "DESCRIPTION: \t" << description << endl;
		for (int i = 0; i < amount.size(); i++){
			cout << dateITS(date[i]) << "\tAMOUNT: " << amount[i] << "\tCREDIT? " << debit_credit[i] << endl; 
		}
		return;
	}

};

class littleAccountant
{
	map<string, account> theBook;
	vector<accountSummary> requested_records;
	int recordCredit = 0;
	int recordDebit = 0;
public:
	// littleAccountant();


	void addTrans(string ACCOUNT, string PERIOD, string CREDIT, string DEBIT){
		theBook[ACCOUNT].addTransaction(PERIOD, CREDIT, DEBIT);
		return;
	}

	void updateInfo(string DESCRIPTION, string ACCOUNT){
		DESCRIPTION.pop_back();
		theBook[ACCOUNT].updateDescriptionID(DESCRIPTION, ACCOUNT);
		return;
	}

	vector<accountSummary> collectRecords(string ACCOUNT_START, string ACCOUNT_END, string PERIOD_START, string PERIOD_END){
		map<string, account>::iterator begin;
		map<string, account>::iterator end;
		begin =  (ACCOUNT_START == "*") ? theBook.begin() : theBook.find(ACCOUNT_START);
		end = (ACCOUNT_END == "*") ? theBook.end() : theBook.end();
		for( ; begin != end; ++begin){
			accountSummary pulledAccount = begin->second.lookInTheBooks(PERIOD_START, PERIOD_END);
			if (pulledAccount.debit != -1){
				requested_records.push_back(pulledAccount);
				recordCredit += pulledAccount.credit;
				recordDebit += pulledAccount.debit;
			}
		}
		return requested_records;
	}
	void printRequestedRecords(){
		for (int i = 0; i < requested_records.size(); i++){
			cout << requested_records[i].accountID << "\t\t" << requested_records[i].description << "\t\t"
				<< requested_records[i].debit << "\t\t" << requested_records[i].credit << "\t\t" << (requested_records[i].debit - requested_records[i].credit) << endl;

		}
	}
	// ~littleAccountant();

};


int main(){
	littleAccountant bigAFBook;
	ifstream infile;
	map<string, account> theBook;
	string ACCOUNT, PERIOD, DEBIT, CREDIT;
	infile.open("accountJournal.txt");
	getline(infile, ACCOUNT);//just reading in the title 
	while ( getline(infile, ACCOUNT, ';') && getline(infile, PERIOD, ';') 
		&& getline(infile, DEBIT, ';') && getline(infile, CREDIT)){
		bigAFBook.addTrans(ACCOUNT, PERIOD, CREDIT, DEBIT);
	}
	infile.close();
	infile.open("accountLabel.txt");
	getline(infile, ACCOUNT);
	while( getline(infile, ACCOUNT, ';') && getline(infile, PERIOD)){
		bigAFBook.updateInfo(PERIOD, ACCOUNT);
	}
	infile.close();
	string account_start, account_end, period_start, period_end, output_type;
	cin >> account_start >> account_end >> period_start >> period_end >> output_type;
	bigAFBook.collectRecords(account_start, account_end, period_start, period_end);
	bigAFBook.printRequestedRecords();
}