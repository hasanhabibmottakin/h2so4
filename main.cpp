#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

size_t w(void* c, size_t s, size_t n, void* u) {
    ((string*)u)->append((char*)c, s * n);
    return s * n;
}

string g(const string& u) {
    CURL* c1;
    CURLcode r;
    string b;

    c1 = curl_easy_init();
    if (c1) {
        curl_easy_setopt(c1, CURLOPT_URL, u.c_str());
        curl_easy_setopt(c1, CURLOPT_WRITEFUNCTION, w);
        curl_easy_setopt(c1, CURLOPT_WRITEDATA, &b);
        curl_easy_setopt(c1, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(c1, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36");
        curl_easy_setopt(c1, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(c1, CURLOPT_SSL_VERIFYHOST, 0L);
        
        r = curl_easy_perform(c1);
        if(r != CURLE_OK) cout << "CE:" << curl_easy_strerror(r) << endl;
        curl_easy_cleanup(c1);
    }
    return b;
}

int main() {
    const char* e1 = getenv("sksdkskdsk");
    const char* e2 = getenv("dkskdsk");
    const char* e3 = getenv("sksdks");

    if (!e1 || !e2 || !e3) { 
        cout << "E:ENV" << endl; 
        return 1; 
    }

    string u1 = e1;
    string u2 = e2;
    string u3 = e3;

    ofstream f("playlist.m3u");
    if (!f.is_open()) { 
        cout << "E:FILE" << endl; 
        return 1; 
    }
    
    f << "#EXTM3U\n";

    string r1 = g(u1);
    if (r1.empty()) { 
        cout << "E:API_EMPTY" << endl; 
        return 1; 
    }

    try {
        json j1 = json::parse(r1);

        for (const auto& i : j1) {
            string c = i.value("code", "");
            string l = i.value("tvg-logo", "");
            string n = i.value("name", "");

            if (!c.empty()) {
                string su1 = u2 + c;
                string r2 = g(su1);
                
                if (!r2.empty()) {
                    try {
                        json j2 = json::parse(r2);
                        string su2 = j2.value("url", "");

                        if (!su2.empty()) {
                            f << "#EXTINF:-1 tvg-logo=\"" << l << "\" group-title=\"Live Cricket\", " << n << "\n";
                            f << "#EXTVLCOPT:http-referrer=" << u3 << "\n";
                            f << su2 << "\n";
                        }
                    } catch (...) {}
                }
            }
        }
    } catch (exception& ex) {
        cout << "E:JSON " << ex.what() << endl;
        return 1;
    }

    f.close();
    return 0;
}
