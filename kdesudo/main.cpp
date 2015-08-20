/***************************************************************************
                          kdesudo.cpp  -  description
                             -------------------
    begin                : Sam Feb 15 15:42:12 CET 2003
    copyright            : (C) 2003 by Robert Gruber
                                       <rgruber@users.sourceforge.net>
                           (C) 2007 by Martin Böhm <martin.bohm@kubuntu.org>
                                       Anthony Mercatante <tonio@kubuntu.org>
                                       Canonical Ltd (Jonathan Riddell
                                                      <jriddell@ubuntu.com>)
                           (C) 2009 by Harald Sitter <apachelogger@ubuntu.com>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <KCmdLineArgs>
#include <KAboutData>
#include <KLocale>
#include <KMessageBox>
#include <KDesktopFile>
#include <KIconLoader>
#include <KIconTheme>
#include <KGlobal>
#include <KStandardDirs>
#include <KProcess>
#include <KDebug>
#include <KApplication>

#include "kdesudo.h"

int main(int argc, char **argv)
{
    KAboutData about(
        "kdesudo", 0, ki18n("KdeSudo"),
        "3.4.2.3", ki18n("Sudo frontend for KDE"),
        KAboutData::License_GPL,
        ki18n("(C) 2007 - 2008 Anthony Mercatante"),
        KLocalizedString(),
        "https://code.launchpad.net/kdesudo/");

    about.setBugAddress("https://launchpad.net/kdesudo/+filebug");

    about.addAuthor(ki18n("Robert Gruber"), KLocalizedString(),
                    "rgruber@users.sourceforge.net", "http://test.com");
    about.addAuthor(ki18n("Anthony Mercatante"), KLocalizedString(),
                    "tonio@ubuntu.com");
    about.addAuthor(ki18n("Martin Böhm"), KLocalizedString(),
                    "martin.bohm@kubuntu.org");
    about.addAuthor(ki18n("Jonathan Riddell"), KLocalizedString(),
                    "jriddell@ubuntu.com");
    about.addAuthor(ki18n("Harald Sitter"), KLocalizedString(),
                    "apachelogger@ubuntu.com");

    KCmdLineArgs::init(argc, argv, &about);

    KCmdLineOptions options;
    options.add("u <runas>", ki18n("sets a runas user"));
    options.add("c <command>", ki18n("The command to execute"));
    options.add("s", ki18n("Forget passwords"));
    options.add("i <icon name>", ki18n("Specify icon to use in the password"
                                       " dialog"));
    options.add("d", ki18n("Do not show the command to be run in the dialog"));
    options.add("p <priority>", ki18n("Process priority, between 0 and 100,"
                                      " 0 the lowest [50]"));
    options.add("r", ki18n("Use realtime scheduling"));
    options.add("f <file>", ki18n("Use target UID if <file> is not writeable"));
    options.add("t", ki18n("Fake option for KDE's KdeSu compatibility"));
    options.add("n", ki18n("Do not keep password"));
    options.add("nonewdcop", ki18n("Use existing DCOP server"));
    options.add("comment <dialog text>", ki18n("The comment that should be "
                "displayed in the dialog"));
    options.add("noignorebutton", ki18n("Do not display « ignore » button"));
    options.add("attach <winid>", ki18n("Makes the dialog transient for an X app specified by winid"));
    options.add("desktop <desktop file>", ki18n("Manual override for "
                "automatic desktop file detection"));

    options.add("+command", ki18n("The command to execute"));

    KCmdLineArgs::addCmdLineOptions(options);
    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

    KApplication a;

    QString executable, arg, command, icon;
    QStringList executableList, commandlist;
    KDesktopFile *desktopFile;

    if (args->isSet("c")) {
        executable = args->getOption("c");
    }

    if (args->count() && executable.isEmpty()) {
        command = args->arg(0);
        commandlist = command.split(" ");
        executable = commandlist[0];
    }

    /* We have to make sure the executable is only the binary name */
    executableList = executable.split(" ");
    executable = executableList[0];

    executableList = executable.split("/");
    executable = executableList[executableList.count() - 1];

    /* Kubuntu has a bug in it - this is a workaround for it */
    KGlobal::dirs()->addResourceDir("apps", "/usr/share/applications/kde");
    KGlobal::dirs()->addResourceDir("apps", "/usr/share/applications/kde4");
    KGlobal::dirs()->addResourceDir("apps", "/usr/share/kde/services");
    KGlobal::dirs()->addResourceDir("apps", "/usr/share/kde4/services");
    KGlobal::dirs()->addResourceDir("apps", "/usr/share/applications");
    KGlobal::dirs()->addResourceDir("apps", "/usr/share/applnk");

    QString path = getenv("PATH");
    QStringList pathList = path.split(":");
    for (int i = 0; i < pathList.count(); i++) {
        executable.remove(pathList[i]);
    }

    if (args->isSet("desktop")) {
        desktopFile = new KDesktopFile(args->getOption("desktop"));
    } else {
        desktopFile = new KDesktopFile(executable + ".desktop");
    }

    /* icon parsing */
    if (args->isSet("i")) {
        icon = args->getOption("i");
    } else {
        QString iconName = desktopFile->readIcon();
        KIconLoader *loader = KIconLoader::global();
        icon = loader->iconPath(iconName, -1 * KIconLoader::StdSizes(
                                    KIconLoader::SizeHuge), true);
    }

    /* generic name parsing */
    QString name = desktopFile->readName();

    a.setQuitOnLastWindowClosed(false);
    KdeSudo kdesudo(icon, name);
    return a.exec();
}
