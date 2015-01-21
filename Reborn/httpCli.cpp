#include <iostream>
#include "curl.h"
#include "stdio.h"
#include "httpCli.h"

using namespace std;

extern "C" std::size_t append_to_string(void* contents, std::size_t size, std::size_t nmemb, void* pstr)
{
	const std::size_t sz = size * nmemb;
	const char* cstr = static_cast<const char*>(contents);
	std::string& str = *static_cast< std::string* >(pstr);
	for (std::size_t i = 0; i < sz; ++i) str += cstr[i];
	return sz;
}

string get(string url)
{
	curl_global_init(CURL_GLOBAL_ALL); // wrap in an RAII shim
	CURL* curl_handle = curl_easy_init(); // use std::unique_ptr with a custom deleter
	std::string page;
	std::string retn;

	curl_easy_setopt(curl_handle, CURLOPT_URL, url); // url
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, append_to_string); // call 'append_to_string' with data

																			// pass the address of string 'page' to the callback 'append_to_string'
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, std::addressof(page));
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "dragonnest.ro"); // user-agent (optional)

	const auto result = curl_easy_perform(curl_handle); // get the page
	if (result == CURLE_OK) 
		retn = page;
	else std::cerr << "**** error: " << curl_easy_strerror(result) << '\n';

	curl_easy_cleanup(curl_handle);
	curl_global_cleanup();
	return retn;
}