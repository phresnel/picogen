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

#include "../../actuarius/actuarius.hh"
#include "../../redshift/include/jobfile.hh"
#include "job.hh"
#include <sstream>
#undef main

#include <QDir>
#include <QFile>
#include <QTextStream>

void prepareJob(RenderJob &, QString const &statusDir) {
        QDir dir;
        if (!dir.exists(statusDir))
                dir.mkdir(statusDir);

        const char *statusFile = (statusDir.toStdString() + "/status").c_str();
        if (!QFile().exists(statusFile)) {
                std::ofstream ofs(statusFile);
                Status status;
                actuarius::OArchive (ofs) & actuarius::pack("status", status);
        }
}


RenderJob* loadFromFile (QFile &f) {
        QTextStream qfs (f.readAll());
        std::stringstream ss;
        ss << qfs.readAll().toStdString();
        RenderJob *job = new RenderJob;

        actuarius::IArchive (ss) & actuarius::pack("job", *job);

        return job;
}

Status readStatus (QString const &statusDir) {
        const char *statusFile = (statusDir.toStdString() + "/status").c_str();
        std::ifstream ifs(statusFile);
        Status status;
        actuarius::IArchive (ifs) & actuarius::pack("status", status);
        return status;
}

void writeStatus (Status const &, QString const &statusDir) {
        const char *statusFile = (statusDir.toStdString() + "/status").c_str();
        std::ofstream ofs(statusFile);
        Status status;
        actuarius::OArchive (ofs) & actuarius::pack("status", status);
}
