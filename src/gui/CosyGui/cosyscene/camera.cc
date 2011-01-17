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

#include "camera.hh"
#include "actuarius/bits/enum.hh"

namespace cosyscene {

const actuarius::Enum<Camera::Kind> Camera::Typenames =
( actuarius::Nvp<Camera::Kind>(Camera::pinhole,      "pinhole")
| actuarius::Nvp<Camera::Kind>(Camera::cylindrical,  "cylindrical")
| actuarius::Nvp<Camera::Kind>(Camera::cubemap_face, "cubemap_face")
);
const actuarius::Enum<CubemapFaceCamera::Face> CubemapFaceCamera::Typenames =
( actuarius::Nvp<CubemapFaceCamera::Face>(CubemapFaceCamera::left,   "left")
| actuarius::Nvp<CubemapFaceCamera::Face>(CubemapFaceCamera::right,  "right")
| actuarius::Nvp<CubemapFaceCamera::Face>(CubemapFaceCamera::front,  "front")
| actuarius::Nvp<CubemapFaceCamera::Face>(CubemapFaceCamera::back,   "back")
| actuarius::Nvp<CubemapFaceCamera::Face>(CubemapFaceCamera::bottom, "bottom")
| actuarius::Nvp<CubemapFaceCamera::Face>(CubemapFaceCamera::top,    "top")
);


PinholeCamera::PinholeCamera() : frontPlaneDistance_ (1) {
}
double PinholeCamera::frontPlaneDistance() const {
        return frontPlaneDistance_;
}
void PinholeCamera::setFrontPlaceDistance(double value) {
        frontPlaneDistance_ = value;
}
bool operator ==(PinholeCamera const&lhs, PinholeCamera const&rhs) {
        return lhs.frontPlaneDistance() == rhs.frontPlaneDistance();
}


CylindricalCamera::CylindricalCamera() : frontPlaneDistance_ (1) {
}
double CylindricalCamera::frontPlaneDistance() const {
        return frontPlaneDistance_;
}
void CylindricalCamera::setFrontPlaceDistance(double value) {
        frontPlaneDistance_ = value;
}
bool operator ==(CylindricalCamera const&lhs, CylindricalCamera const&rhs) {
        return lhs.frontPlaneDistance() == rhs.frontPlaneDistance();
}


CubemapFaceCamera::CubemapFaceCamera() : face_(CubemapFaceCamera::front) {
}
void CubemapFaceCamera::setFace (CubemapFaceCamera::Face face) {
        face_ = face;
}
CubemapFaceCamera::Face CubemapFaceCamera::face() const {
        return face_;
}
bool operator ==(CubemapFaceCamera const&lhs, CubemapFaceCamera const&rhs) {
        return lhs.face() == rhs.face();
}



Camera::Camera() {
        toPinholeCamera();
}

Camera::Kind Camera::kind() const {
        return kind_;
}

void Camera::to (Camera::Kind kind) {
        switch (kind) {
        case pinhole:
                toPinholeCamera();
                return;
        case cylindrical:
                toCylindricalCamera();
                return;
        case cubemap_face:
                toCubemapFaceCamera();
                return;
        }
        throw std::runtime_error(
                        "unsupported Kind in Camera::to(Kind)");
}

void Camera::toPinholeCamera (PinholeCamera const &val) {
        kind_ = pinhole;
        pinholeCamera_ = val;
}

void Camera::toCylindricalCamera (CylindricalCamera const &val) {
        kind_ = cylindrical;
        cylindricalCamera_ = val;
}

void Camera::toCubemapFaceCamera (CubemapFaceCamera const &val) {
        kind_ = cubemap_face;
        cubemapFaceCamera_ = val;
}

PinholeCamera Camera::pinholeCamera () const {
        return pinholeCamera_;
}

CylindricalCamera Camera::cylindricalCamera () const {
        return cylindricalCamera_;
}

CubemapFaceCamera Camera::cubemapFaceCamera () const {
        return cubemapFaceCamera_;
}

bool operator == (Camera const &lhs, Camera const &rhs) {
        if (lhs.kind() != rhs.kind())
                return false;
        switch (lhs.kind()) {
        case Camera::pinhole:
                return lhs.pinholeCamera() == rhs.pinholeCamera();
        case Camera::cylindrical:
                return lhs.cylindricalCamera() == rhs.cylindricalCamera();
        case Camera::cubemap_face:
                return lhs.cubemapFaceCamera() == rhs.cubemapFaceCamera();
        }
        throw std::runtime_error("unsupported kind in bool operator == "
                "(Camera const &lhs, Camera const &rhs)");
}


} // namespace cosyscene
