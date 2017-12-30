
#include "patheditwidget.h"

#include <QLayout>
#include <QCompleter>
#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <QStandardPaths>

PathEditWidget::PathEditWidget(QWidget *parent) : QWidget(parent)
{
    _filesModel = new QFileSystemModel();
    _filesModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    _filesModel->setRootPath("");
    _versionArg = "-v";

    setupWidgets();
    _env = QProcessEnvironment::systemEnvironment();
}

QString PathEditWidget::programm() const
{
    return _programm;
}

void PathEditWidget::setProgramm(const QString &programm)
{
    _programm = programm;
}

QString PathEditWidget::path() const
{
    return _path;
}

void PathEditWidget::setPath(const QString &path)
{
    _path = path;
    if (QDir::fromNativeSeparators(_pathLineEdit->text()) != path)
    {
        _pathLineEdit->setText(QDir::toNativeSeparators(path));
        _filesModel->setRootPath(path);
    }
    checkProgramm();
}

bool PathEditWidget::isValid() const
{
    if (_pathLineEdit->text().isEmpty())
        return true;
    return false;
}

void PathEditWidget::setEnv(QProcessEnvironment env)
{
    _env = env;
    checkProgramm();
}

void PathEditWidget::buttonClicked()
{
    QString dir =QFileDialog::getExistingDirectory(this, tr("Choose existing directory"), _pathLineEdit->text());
    if (!dir.isEmpty())
        setPath(dir);
}

void PathEditWidget::checkProgramm()
{
#if defined(Q_OS_WIN)
    QChar listSep = ';';
#else
    QChar listSep = ':';
#endif
    QString programm = _programm;
    QProcess *process = new QProcess();
    if (_programm.isEmpty())
        return;
    QProcessEnvironment env = _env;
    if (!_path.isEmpty())
        env.insert("PATH", _path + listSep + env.value("PATH"));
    QString path = QStandardPaths::findExecutable(_programm, env.value("PATH").split(listSep));
    if (!path.isEmpty())
        programm = path;
    process->setProcessEnvironment(env);
    QStringList args;
    args.append(_versionArg);

    process->start(programm, args);
    if (!process->waitForFinished(3000))
    {
        _labelVersion->setText(tr("<span style=\"color: red;\">%1 not found: %2</span>").arg(_programm).arg(process->errorString()));
        delete process;
        return;
    }
    QString version = process->readLine();
    _labelVersion->setText(tr("%1 in path '%2'").arg(version).arg(QDir::toNativeSeparators(path)));
    delete process;
    return;
}

void PathEditWidget::checkLineEdit()
{
    setPath(_pathLineEdit->text());
}

QString PathEditWidget::versionArg() const
{
    return _versionArg;
}

void PathEditWidget::setVersionArg(const QString &versionArg)
{
    _versionArg = versionArg;
}

void PathEditWidget::setupWidgets()
{
    QLayout *layout = new QVBoxLayout();

    QLayout *layout2 = new QHBoxLayout();

    _pathLineEdit = new QLineEdit();
    _pathLineEdit->setPlaceholderText(tr("Leave empty to use system PATH"));
    _pathLineEdit->setCompleter(new QCompleter(_filesModel));
    connect(_pathLineEdit, SIGNAL(editingFinished()), this, SLOT(checkLineEdit()));
    layout2->addWidget(_pathLineEdit);

    _button = new QToolButton();
    _button->setText("...");
    connect(_button, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
    layout2->addWidget(_button);

    layout->addItem(layout2);

    _labelVersion = new QLabel("");
    _labelVersion->setTextInteractionFlags(Qt::TextSelectableByMouse);
    _labelVersion->setCursor(Qt::IBeamCursor);
    layout->addWidget(_labelVersion);

    setLayout(layout);
}
