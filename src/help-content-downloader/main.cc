//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010  Sebastian Mach (*1983)
// * mail: phresnel/at/gmail/dot/com
// * http://phresnel.org
// * http://picogen.org
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <iostream>
#include <fstream>

#include <QtGui/QApplication> // we need this as QWebPage doesn't work in pure
                              // console apps, even if we don't display anything
#include <QDomDocument>
#include <QWebElement>
#include <QWebFrame>
#include <QWebPage>
#include <QFile>
#include <QTextStream>

#include "download.hh"

// I should prolly look into w3 standards
enum UrlType { internal, relative_local, absolute_local, external };
UrlType getUrlType (const QString &url) {
        if (url.startsWith("#")) {
                //std::clog << "internl: " << url.toStdString() << "\n";
                return internal;
        } else if (url.startsWith("/wiki")) {
                //std::clog << "rel loc: " << url.toStdString() << "\n";
                return relative_local;
        } else if (url.startsWith("http://picogen.org/wiki")) {
                //std::clog << "abs loc: " << url.toStdString() << "\n";
                return absolute_local;
        } else {
                //std::clog << "externl: " << url.toStdString() << "\n";
                return external;
        }
}



struct DownloadAppointment {
        QString url;
        enum Type { Article, Image } type;

        DownloadAppointment() {}
        DownloadAppointment (QString url, Type type) : url(url), type(type) {}
};
bool operator == (DownloadAppointment const &lhs, DownloadAppointment const &rhs) {
        return lhs.type == rhs.type && lhs.url == rhs.url;
}



QString localizeUrl (const DownloadAppointment &dapp) {
        switch(dapp.type) {
        case DownloadAppointment::Article: {
                        QRegExp titleRegEx (QString("index\\.php\\?title=(.+$)"));

                        if (!dapp.url.contains("&")         // disallow ...
                         && !dapp.url.contains("action=")   // ... some ...
                         && !dapp.url.contains("redlink=1") // ... specials
                         && !dapp.url.contains("User:")
                         && !dapp.url.contains("User_talk:")
                         && !dapp.url.contains("Special:")
                         && titleRegEx.indexIn(dapp.url) > -1
                        ) {
                                QString title = titleRegEx.cap(1);
                                title.replace("/", "-").replace(":","_");
                                const QString filename = title + ".html";
                                return filename;
                        } else {
                                return dapp.url;
                        }
                }
                break;
        case DownloadAppointment::Image: {
                        QString title = dapp.url;
                        title.replace("/", "-").replace(":","_");
                        const QString filename = title;
                        return filename;
                }
                break;
        }
}



QList<DownloadAppointment> mogrify (const DownloadAppointment &dapp) {
        std::cout << "downloading+processing " << dapp.url.toStdString() << " ... " << std::flush;

        QList<DownloadAppointment> downloadSchedule;
        const QString localizedUrl = localizeUrl(dapp);
        if (dapp.url == localizedUrl) {
                std::cout << "ignored\n";
                return downloadSchedule;
        }

        const Download download (dapp.url.toStdString().c_str());

        if (dapp.type == DownloadAppointment::Article) {

                QWebPage p;
                p.mainFrame()->setHtml(download.toString().c_str());

                p.mainFrame()->findFirstElement("div#column-one").removeFromDocument();
                p.mainFrame()->findFirstElement("div#footer").removeFromDocument();
                p.mainFrame()->findFirstElement("div#jump-to-nav").removeFromDocument();
                p.mainFrame()->findFirstElement(".printfooter").removeFromDocument();
                foreach (QWebElement e, p.mainFrame()->findAllElements("style")) {
                        e.removeFromDocument();
                }
                foreach (QWebElement e, p.mainFrame()->findAllElements("meta")) {
                        e.removeFromDocument();
                }
                foreach (QWebElement e, p.mainFrame()->findAllElements("script")) {
                        e.removeFromDocument();
                }

                // This is totally braindead, but it seems that
                // any attempt to add new children fails.
                bool first = true;
                foreach (QWebElement e, p.mainFrame()->findAllElements("link")) {
                        if (first) {
                                e.setInnerXml("");
                                e.setAttribute("rel", "stylesheet");
                                e.setAttribute("type", "text/css");
                                e.setAttribute("href", "style/main.css");
                                first = false;
                        } else {
                                e.removeFromDocument();
                        }
                }


                QWebElement content = p.mainFrame()->findFirstElement("div#content");
                QWebElementCollection aElems = content.findAll("a");
                foreach (QWebElement e, aElems) {
                        const QString href = e.attribute("href");
                        const UrlType type = getUrlType (href);
                        if (type == absolute_local || type == relative_local) {
                                QString const localHref = localizeUrl(DownloadAppointment(href,DownloadAppointment::Article));
                                e.setAttribute("href", localHref);

                                const DownloadAppointment d (
                                        ((type == relative_local)?"http://picogen.org":"") + href, DownloadAppointment::Article);
                                if (!QFile::exists("help-content/" + localHref) && !downloadSchedule.contains(d)) {
                                        downloadSchedule.push_back(d);
                                }
                        }
                }

                QWebElementCollection imgElems = content.findAll("img");
                foreach (QWebElement e, imgElems) {
                        const QString href = e.attribute("src");
                        const UrlType type = getUrlType (href);

                        QString const localHref =
                                localizeUrl(DownloadAppointment(
                                        ((type == relative_local)?"http://picogen.org":"") + href,
                                        DownloadAppointment::Image));
                        e.setAttribute("src", "media/" + localHref); // <-- relative to article


                        QString d;
                        if (type == relative_local)
                                d = "http://picogen.org" + href;
                        else
                                d = href;
                        if (!QFile::exists("help-content/media/" + localHref) && !downloadSchedule.contains(DownloadAppointment(d, DownloadAppointment::Image))) {
                                downloadSchedule.push_back(DownloadAppointment(d, DownloadAppointment::Image));
                        }
                }


                const QString filename = "help-content/" + localizedUrl;
                std::cout << " as " << filename.toStdString() << " ... " << std::flush;

                QFile file(filename);
                if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                        std::cerr << "error upon opening " << filename.toStdString() << "\n";
                } else {
                        QTextStream out(&file);
                        out << p.mainFrame()->toHtml();
                        std::cout << "done\n";
                }
        } else {
                const QString localizedUrl = localizeUrl(dapp);
                if (dapp.url != localizedUrl) {
                        const QString filename = "foo/media/" + localizedUrl;
                        std::cout << " as " << filename.toStdString() << " ... " << std::flush;
                        std::ofstream ofs (filename.toStdString().c_str(), std::ios_base::out | std::ios_base::binary);
                        foreach (char c, download.content())
                                ofs << c;
                        std::cout << "done\n";
                } else {
                        std::cout << "not downloaded\n";
                }
        }

        return downloadSchedule;
}

int main(int argc, char *argv[])
{
        //QCoreApplication a(argc, argv);
        QApplication a(argc, argv);
        curl_global_init(CURL_GLOBAL_ALL);

        QList<DownloadAppointment> next;
        next.push_back(DownloadAppointment("http://picogen.org/wiki/index.php?title=Main_Page", DownloadAppointment::Article));
        //next.push_back("http://picogen.org/wiki/index.php?title=References");
        //next.push_back("http://picogen.org/wiki/index.php?title=Examples_and_Interesting_Programs_for_mkheightmap");
        //next.push_back(DownloadAppointment("http://picogen.org/wiki/index.php?title=Examples_and_Interesting_Programs_for_mkheightmap",DownloadAppointment::Article));
        do {
                QList<DownloadAppointment> newSched;
                while (!next.empty()) {
                        QList<DownloadAppointment> tmp = mogrify (next.front());
                        foreach (DownloadAppointment t, tmp) newSched.push_back(t);
                        next.pop_front();
                }
                next = newSched;

                std::clog << "processing next " << next.size() << " items.\n";
        } while (next.size());

        curl_global_cleanup();
}
