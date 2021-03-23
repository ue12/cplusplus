#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>

#include "json/json.hpp"
using json = nlohmann::json;

size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*)ptr, size * nmemb);
    return size * nmemb;
}

std::string get_request( const std::string& url ){
    auto curl = curl_easy_init();
    std::string response_string;
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        
        std::string header_string;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);

        CURLcode res = curl_easy_perform(curl);
        if(res != CURLE_OK){
            std::cout << "curl_easy_perform() failed:\n";
            std::cout << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
        curl = NULL;
    }
    return response_string;
}


int parse_response(const std::string& res){
    auto j = json::parse(res);
    return j["response"]["numFound"];
}

int main(int argc, char** argv) {
    curl_global_init(CURL_GLOBAL_DEFAULT);

    std::vector<std::string> urls = {
        "http://api.archives-ouvertes.fr/search/?q=authIdHal_s:bmarchand&fl=label_s,uri_s,halId_s&wt=json",
        "http://api.archives-ouvertes.fr/search/?q=authIdHal_s:jacques-besson&fl=label_s,uri_s,halId_s&wt=json",
        "http://api.archives-ouvertes.fr/search/?q=authIdHal_s:samuel-forest&fl=label_s,uri_s,halId_s&wt=json",
        "http://api.archives-ouvertes.fr/search/?q=authIdHal_s:valerie-roy&fl=label_s,uri_s,halId_s&wt=json"
    };

    for( auto url: urls){
        std::string out = get_request( url );
        std::cout << "numFound = " << parse_response(out) << std::endl;
    }
    
    curl_global_cleanup();

    return 0; 
}