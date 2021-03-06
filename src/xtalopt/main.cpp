/**********************************************************************
  main.cpp - The main() function to be used by XtalOpt 11.0 and beyond.

  Copyright (C) 2016-2017 by Patrick S. Avery

  This source code is released under the New BSD License, (the "License").

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

 ***********************************************************************/

#include <QApplication>
#include <QCommandLineParser>

#include <xtalopt/cliOptions.h>
#include <xtalopt/ui/dialog.h>
#include <xtalopt/xtalopt.h>

int main(int argc, char* argv[])
{
  // Set up groups for QSettings
  QCoreApplication::setOrganizationName("XtalOpt");
  QCoreApplication::setOrganizationDomain("xtalopt.github.io");
  QCoreApplication::setApplicationName("XtalOpt");
  QCoreApplication::setApplicationVersion("11.0");

  QApplication app(argc, argv);

  QCommandLineParser parser;
  parser.setApplicationDescription("XtalOpt: an open-source evolutionary "
                                   "algorithm for crystal structure "
                                   "prediction");
  parser.addHelpOption();
  parser.addVersionOption();

  QCommandLineOption cliModeOption(
      QStringList() << "cli",
          QCoreApplication::translate(
              "main",
              "Use the command-line interface (CLI) mode."
          )
  );
  parser.addOption(cliModeOption);

  QCommandLineOption cliResumeOption(
      QStringList() << "resume",
          QCoreApplication::translate(
              "main",
              "Resume an XtalOpt run in CLI mode."
          )
  );
  parser.addOption(cliResumeOption);

  QCommandLineOption inputFileOption(
      QStringList() << "input-file",
          QCoreApplication::translate(
              "main",
              "Specify the input file for CLI mode."
          ),
          QCoreApplication::translate("main", "file")
  );
  inputFileOption.setDefaultValue("xtalopt.in");
  parser.addOption(inputFileOption);

  QCommandLineOption plotModeOption(
      QStringList() << "plot",
          QCoreApplication::translate(
              "main",
              "Show a plot of a specified XtalOpt directory."
          )
  );
  parser.addOption(plotModeOption);

  QCommandLineOption dataDirOption(
      QStringList() << "dir",
          QCoreApplication::translate(
              "main",
              "Specify the XtalOpt results directory to be used for a CLI "
              "resume or a plot."
          ),
          QCoreApplication::translate("main", "directory")
  );
  parser.addOption(dataDirOption);

  parser.process(app);
  bool cliMode = parser.isSet(cliModeOption);
  bool cliResume = parser.isSet(cliResumeOption);
  bool plotMode = parser.isSet(plotModeOption);

  QString inputfile = parser.value(inputFileOption);
  QString dataDir = parser.value(dataDirOption);

  // Make sure we have valid options set...
  if (plotMode && !parser.isSet(dataDirOption)) {
    qDebug() << "To use plot mode, you must specify an XtalOpt results"
             << "directory with --dir";
    return 1;
  }

  if (cliResume && !parser.isSet(dataDirOption)) {
    qDebug() << "To resume an XtalOpt run in CLI mode, you must specify an"
             << "XtalOpt results directory with --dir";
    return 1;
  }

  if (plotMode && cliMode) {
    qDebug() << "Error: you cannot use CLI mode and plot mode"
             << "at the same time!";
    return 1;
  }

  if (plotMode && cliResume) {
    qDebug() << "Error: you cannot resume in CLI mode and use plot mode"
             << "at the same time!";
    return 1;
  }

  // It would be nice if sometime in the future we didn't have to create
  // all the dialogs for a run that doesn't use the GUI. However, it is
  // deeply integrated and hard to do now, so we are going to create
  // the dialogs anyways
  XtalOpt::XtalOptDialog d;

  if (cliMode) {
    XtalOpt::XtalOpt& xtalopt =
      *qobject_cast<XtalOpt::XtalOpt*>(d.getOptBase());
    xtalopt.setUsingGUI(false);

    if (!XtalOpt::XtalOptCLIOptions::readOptions(inputfile, xtalopt))
      return 1;
    if (!xtalopt.startSearch())
      return 1;
  }
  // We just want to generate a plot tab and display it...
  else if (plotMode) {
    XtalOpt::XtalOpt& xtalopt =
      *qobject_cast<XtalOpt::XtalOpt*>(d.getOptBase());
    if (!xtalopt.plotDir(dataDir))
      return 1;
    d.beginPlotOnlyMode();
  }
  else if (cliResume) {
    XtalOpt::XtalOpt& xtalopt =
      *qobject_cast<XtalOpt::XtalOpt*>(d.getOptBase());
    xtalopt.setUsingGUI(false);

    // Make sure the state file exists
    if (!QDir(dataDir).exists("xtalopt.state")) {
      qDebug() << "Error: no xtalopt.state file found in"
               << dataDir;
      qDebug() << "Please check your --dir option and try again";
      return 1;
    }

    // Try to load the state file
    if (!xtalopt.load(QDir(dataDir).filePath("xtalopt.state")))
      return 1;

    // Warn the user if they need to change something to get XtalOpt to run
    if (xtalopt.limitRunningJobs && xtalopt.runningJobLimit == 0) {
      qDebug() << "Warning: the running job limit is set to zero. You can"
               << "change this in the runtime options file in the local"
               << "working directory";
    }
    if (xtalopt.contStructs == 0) {
      qDebug() << "Warning: the continuous structure limit is set to zero. You"
               << "can change this in the runtime options file in the local"
               << "working directory";
    }

    // If the runtime file doesn't exist, write one
    if (!QFile(xtalopt.CLIRuntimeFile()).exists())
      XtalOpt::XtalOptCLIOptions::writeInitialRuntimeFile(xtalopt);

    xtalopt.emitStartingSession();
    xtalopt.emitSessionStarted();
  }
  // If we are using the GUI, show the dialog...
  else {
    d.show();
  }

  return app.exec();
}
