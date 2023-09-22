#include <curl/curl.h>
#include <rapidjson/document.h>
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

string JsonRet;

size_t WriteTO ( char * ptr, size_t size, size_t nmemb, void * userData ) {

	JsonRet += ptr;
	return 0;
}

// Get Decimal Value
// just for shortCuts
double GDV ( rapidjson::Document& doc, const char * look ) {

	return doc [ "current" ] [ look ].GetDouble ();
}

int main ( int argc, char ** argv ) {

	// making connection
	CURL * c = curl_easy_init ();
	if ( !c ) {

		printf ( "Something went wrong!" );
		return -1;
	}

	string apiKey = /* Your API Key! */;
	string location;
	string request;
	cout << "put the desired city weather: ";
	getline ( cin, location );

	request = "http://api.weatherapi.com/v1/current.json?key=" + apiKey + "&q=" + location + "&api=no";

	// calling the API
	curl_easy_setopt ( c, CURLOPT_URL, request.c_str () );
	curl_easy_setopt ( c, CURLOPT_WRITEFUNCTION, WriteTO );
	int res = curl_easy_perform ( c );

	// cout << JsonRet << endl;

	// cleanup
	curl_easy_cleanup ( c );

	// parse a the returned json document!
	rapidjson::Document doc;

	if ( doc.Parse ( JsonRet.c_str () ).HasParseError () ) {

		// fixing the bug which is adding some extra non json content to the end of the string
		// so the solution is to end the string when the json ends

		size_t found = JsonRet.find_last_of ( "}}" );
		JsonRet [ found + 2 ] = 0;

		// checking the errors again
		if ( doc.Parse ( JsonRet.c_str () ).HasParseError () ) {

			cout << "Invalid Given location!" << endl;
			return -1;
		}
	}

	// Getting and printing the values of the Parsed JSON content
	cout << "Location: " << doc [ "location" ] [ "country" ].GetString () << "/" << doc [ "location" ] [ "name" ].GetString () << endl;
	cout << "Weather: " << endl;
	cout << "  temp: " << GDV ( doc, "temp_c" ) << "C" << endl;
	cout << "  feels like: " << GDV ( doc, "feelslike_c" ) << "C" << endl;
	cout << "  wind speed: " << GDV ( doc, "wind_kph" ) << "km/h" << endl;
	cout << "  wind direction: " << GDV ( doc, "wind_degree" ) << " degree" << endl;
	cout << "  pressure: " << GDV ( doc, "pressure_mb" ) << "milibar" << endl;
	cout << "  wind gust: " << GDV ( doc, "gust_kph" ) << "km/h" << endl;
	cout << "  humidity: " << GDV ( doc, "humidity" ) << "%" << endl;
	cout << "  clouds: " << GDV ( doc, "cloud" ) << "%" << endl;
	cout << "  UV index: " << GDV ( doc, "uv" ) << endl;
	cout << "Last updated: " << doc [ "current" ] [ "last_updated" ].GetString () << endl;
	return 0;
}
