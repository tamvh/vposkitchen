#ifndef CDOWNLOADFILEMAN_H
#define CDOWNLOADFILEMAN_H

#include <QObject>
#include <QList>
#include <QTime>
#include <QString>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

//http://www.bogotobogo.com/Qt/Qt5_QNetworkRequest_Http_File_Download.php

typedef struct tagDownFileInfo{
    QString qstrUrl;
    QString qstrLocalFile;
    //QTime stLMTime;
    qulonglong qullLMTime;
    int nIdImage;
    bool bSetCache;
    bool bResult;
    tagDownFileInfo(){
        bResult = false;
        qullLMTime = 0;
        bSetCache = true;
        nIdImage = 0;
    }
} DOWNFILE_INFO, *LPDOWNFILE_INFO;

class CDownloadFileMan : public QObject
{
    Q_OBJECT
public:
    explicit CDownloadFileMan(QObject *parent = 0);
    ~CDownloadFileMan();
    void Reset();
    void AddDownLoadFile(const DOWNFILE_INFO& dfi);
    void StartDownload();
    void SetData(const QString& qstrData);
    bool IsRunFirstTime();
    QString GetData();
    QList<DOWNFILE_INFO>& GetListFile();
signals:
    void NotifyDownloadNextFile();
    void NotifyDownloadDone();
    void NotifyComplete(const QString& data);
private:
    void DownloadOneFileFinish();
    bool IsFileCached();
    void DeleteOldFiles();
private slots:
    void HttpReadyRead();
    void HttpDownloadFinished();
    void DownloadNextFile();
    void DownloadDone();
private:
    QList<DOWNFILE_INFO> m_ListFile;
    QList<int> m_ListImageLoaded;
    QList<int> m_ListImageModified;
    int m_nCurDLIndex;
    QFile *m_qFile;
    QNetworkReply *m_qReply;
    QNetworkAccessManager *m_qNetAccessManager;
    QString m_qstrData;
    bool m_bRunFirstTime;
};

#endif // CDOWNLOADFILEMAN_H
