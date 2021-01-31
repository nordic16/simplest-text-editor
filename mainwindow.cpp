#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QIODevice>
#include <QClipboard>

void WriteToFile(QString& path, const QString &content);
QString ReadFromFile(QString& path);
void UpdateClipboard(const QString& text);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

////<events>

/// Setting the file path to an empty string will force
/// the app to create a new file instead of saving it
/// to an already existing one.
void MainWindow::on_actionNew_triggered()
{
    this->FPATH = "";
    ui->TextBox->setText("");
}

/// Creates a new file if FPATH is empty.
void MainWindow::on_actionSave_triggered()
{
    QString path = this->FPATH;

    // If FPATH is empty, create a new file, otherwise use FPATH as the path
    // where the content of TextBox is gonna be written to.
    if (path.isEmpty()) {
        path = QFileDialog::getSaveFileName(this, "Save File", "file", "Text files(*.txt)");
    }

    WriteToFile(path, ui->TextBox->toPlainText());
    this->FPATH = path;

    // Gets the last portion of the string separated by a '\' (the file name).
    QString fname = path.section("/", -1);
    this->setWindowTitle(fname.append(" - Simple Text Editor"));
}

/// Saves content to a new file regardless of path being empty or not.
void MainWindow::on_actionSave_As_triggered()
{
    QString path = QFileDialog::getSaveFileName(this, "Save File", "file", "Text files(*.txt)");
    WriteToFile(path, ui->TextBox->toPlainText());

    // Gets the last portion of the string separated by a '\' (the file name).
    QString fname = path.section("/", -1);
    this->setWindowTitle(fname.append(" - Simple Text Editor"));
}

/// Opens a new file, copying its content to TextBox.
void MainWindow::on_actionOpen_triggered()
{
    // If the user clicks on cancel, a null string is returned.
    QString path = QFileDialog::getOpenFileName(this, "Open", "", "Text files(*.txt)");

    if (path.isNull()) {
        return;
    }

    this->FPATH = path;

    // Retrieves the content from path, then puts them on TextBox.
    QString lines = ReadFromFile(this->FPATH);

    ui->TextBox->setText(lines);
}

/// Copies the content of TextBox to the clipboard.
void MainWindow::on_actionCopy_triggered()
{
    UpdateClipboard(ui->TextBox->toPlainText());
}

/// Copies the content of TextBox to the clipboard.
/// Clearing the TextBox.
void MainWindow::on_actionCut_triggered()
{
    UpdateClipboard(ui->TextBox->toPlainText());
    ui->TextBox->setText("");
}

/// Pastes the content of the clipboard to TextBox.
void MainWindow::on_actionPaste_triggered()
{
    QClipboard* board = QGuiApplication::clipboard();
    ui->TextBox->setText(board->text());
}
////</events>


////<Helper functions>
/// Writes stuff to a file stored on path.
void WriteToFile(QString& path, const QString& content)
{
    QFile file(path);

    // If the file was opened successfully, write content to it.
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << content;
    }
}

/// Reads stuff from a file stored on path.
QString ReadFromFile(QString& path)
{
    QFile file(path);
    QString val;

    // If the file was opened successfully, read all lines.
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        stream >> val;
    }

    return val;
}

void UpdateClipboard(const QString& text) {
    QClipboard* clipboard = QGuiApplication::clipboard();
    clipboard->setText(text);
}
////</Helper functions>
