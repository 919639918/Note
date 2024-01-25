#include "mainwindow.h"
#include "quuid.h"
#include <QApplication>
#include <QDir>

bool qCopyDirectory(const QDir &fromDir, const QDir &toDir, bool bCoverIfFileExists)
{
    QDir formDir_ = fromDir;
    QDir toDir_ = toDir;

    if (!toDir_.exists())
    {
        if (!toDir_.mkdir(toDir.absolutePath()))
            return false;
    }

    QFileInfoList fileInfoList = formDir_.entryInfoList();
    foreach (QFileInfo fileInfo, fileInfoList)
    {
        if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        // 拷贝子目录
        if (fileInfo.isDir())
        {
            // 递归调用拷贝
            if (!qCopyDirectory(fileInfo.filePath(), toDir_.filePath(fileInfo.fileName()), true))
                return false;
        }
        // 拷贝子文件
        else
        {
            if (bCoverIfFileExists && toDir_.exists(fileInfo.fileName()))
            {
                toDir_.remove(fileInfo.fileName());
            }
            if (!QFile::copy(fileInfo.filePath(), toDir_.filePath(fileInfo.fileName())))
            {
                return false;
            }
        }
    }
    return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QUuid uuid = QUuid::createUuid();
    qDebug() << uuid.toString();

    QString dest = "./config/public/" + uuid.toString();
    QString path = "./config/基础级/";
    QString path_2 = "./config/上面级/";
    QDir dir;
    if (!dir.exists(dest))
    {
        dir.mkpath(dest);
    }

    qCopyDirectory(path, dest, true);
    qCopyDirectory(path_2, dest, true);

    return 0;
}
