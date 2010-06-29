
#ifndef DOWNLOAD_HH_INCLUDED_20100511
#define DOWNLOAD_HH_INCLUDED_20100511

#include <curl/curl.h>
#include <vector>
#include <stdexcept>

class Download {
public:
        inline Download (const char* url);
        bool contentTypeAvailable () const { return contentTypeDownloaded_; }
        bool contentDownloaded () const { return dataDownloaded_; }

        std::vector<char> content() const { return content_; }
        std::string contentType() const { return type; }

        std::string toString () const {
                std::string ret;
                for (int i=0; i<content_.size(); ++i)
                        ret += content_[i];
                return ret;
        }
private:
        std::string type;
        std::vector<char> content_;
        std::string dataError, typeError;

        bool contentTypeDownloaded_, dataDownloaded_;

        static size_t write_data(void *buffer, size_t size, size_t nmemb, void *data) {
                for (size_t i=0; i<nmemb; ++i) {
                        ((std::vector<char>*)data)->push_back(((char*)buffer)[i]);
                }
                return nmemb;
        }
};

inline Download::Download (const char *url) {
        char curlError[CURL_ERROR_SIZE];

        CURL *curl = curl_easy_init();
        if (curl == NULL) {
                throw std::runtime_error ("curl_easy_init() failed");
        }

        curl_easy_setopt(curl, CURLOPT_URL, url);


        // Get type.
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
        curl_easy_perform(curl);
        char *info;
        curl_easy_getinfo (curl, CURLINFO_CONTENT_TYPE, &info);
        if (info) {
                this->type = std::string(info);
                contentTypeDownloaded_ = true;
        } else {
                typeError = std::string(curlError);
                contentTypeDownloaded_ = false;
        }


        // Download
        std::vector<char> data;
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curlError);

        const CURLcode success = curl_easy_perform(curl);
        if (0 == success) {
                this->content_ = data;
                dataDownloaded_ = true;
        } else {
                dataError = std::string(curlError);
                dataDownloaded_ = false;
        }


        curl_easy_cleanup (curl);
}

#endif // DOWNLOAD_HH_INCLUDED_20100511
