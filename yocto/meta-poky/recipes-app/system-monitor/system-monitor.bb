SUMMARY = "C++ System Monitor Application"
DESCRIPTION = "A terminal-based C++ application to monitor system resources like CPU, memory, and processes using Ncurses."
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=bdd6ede95059016d7a51a39d5d2ed13f"

SRC_URI = "git://github.com/Barazii/cpp-system-monitor.git;protocol=ssh;branch=main"
SRCREV = "ecfd87a8a26591c81686e631cab33fa5ff3854a3"

S = "${WORKDIR}/git"

inherit cmake

DEPENDS = "ncurses boost"

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${B}/monitor ${D}${bindir}/
}