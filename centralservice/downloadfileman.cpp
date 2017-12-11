#include <sys/types.h>
#include <sys/time.h>
#include <utime.h>
#include <QFileInfo>
#include <QDir>
#include <QUrl>
#include <QDebug>
#include <QDirIterator>
#include "downloadfileman.h"

CDownloadFileMan::CDownloadFileMan(QObject *parent) : QObject(parent)
{
    m_nCurDLIndex = 0;
    m_qFile = NULL;
    m_qReply = NULL;
    m_qNetAccessManager = NULL;
    m_bRunFirstTime = true;

    connect(this, SIGNAL(NotifyDownloadNextFile()), this, SLOT(DownloadNextFile()), Qt::QueuedConnection);
    connect(this, SIGNAL(NotifyDownloadDone()), this, SLOT(DownloadDone()), Qt::QueuedConnection);
}

CDownloadFileMan::~CDownloadFileMan(){

}

void CDownloadFileMan::Reset(){
    m_nCurDLIndex = 0;
    m_ListFile.clear();
}

void CDownloadFileMan::StartDownload(){
    m_nCurDLIndex = 0;
    m_qNetAccessManager = new QNetworkAccessManager(this);
    DownloadNextFile();
}

void CDownloadFileMan::AddDownLoadFile(const DOWNFILE_INFO& dfi){
    m_ListFile.push_back(dfi);
}

void CDownloadFileMan::DownloadNextFile(){
    if (m_nCurDLIndex >= m_ListFile.count()){
        emit NotifyDownloadDone();
        return;
    }
    else{
        if (m_qFile){
            m_qFile->flush();
            m_qFile->close();
            delete m_qFile;
            m_qFile = NULL;
        }
        if (m_qReply){
            delete m_qReply;
            m_qReply = NULL;
        }
        /*
        if (m_qNetAccessManager){
            delete m_qNetAccessManager;
            m_qNetAccessManager = NULL;
        }
        */

        if (IsFileCached()){
            m_ListFile[m_nCurDLIndex].bResult = true;
            if (!m_ListImageLoaded.contains(m_ListFile[m_nCurDLIndex].nIdImage)){
                m_ListImageLoaded.push_back(m_ListFile[m_nCurDLIndex].nIdImage);
            }
            m_ListFile[m_nCurDLIndex].bSetCache = !m_ListImageModified.contains(m_ListFile[m_nCurDLIndex].nIdImage);
            m_nCurDLIndex++;
            emit NotifyDownloadNextFile();
            return;
        }
        DeleteOldFiles();
        m_ListFile[m_nCurDLIndex].bResult = false;
        QFileInfo fi(m_ListFile[m_nCurDLIndex].qstrLocalFile);
        QDir qDir;
        if (!qDir.mkpath(fi.absoluteDir().absolutePath())){
            qDebug() << "=== mkpath : " <<  fi.absoluteDir().absolutePath() <<  " failed\n";
            DownloadOneFileFinish();
            return;
        }
        m_qFile = new QFile(fi.absoluteFilePath());
        if (!m_qFile->open(QIODevice::WriteOnly)){
            delete m_qFile;
            m_qFile = NULL;
            DownloadOneFileFinish();
            qDebug() << "=== new file: " <<  fi.absoluteFilePath() <<  " failed\n";
            return;
        }
        //qDebug() << "=== Download Image: " << m_ListFile[m_nCurDLIndex].qstrUrl <<  " to file " << m_ListFile[m_nCurDLIndex].qstrLocalFile << "\n";
        /*
        m_qNetAccessManager = new QNetworkAccessManager(this);
        if (!m_qNetAccessManager){
            m_qFile->close();
            m_qFile->remove();
            delete m_qFile;
            m_qFile = NULL;
            qDebug() << "=== new QNetworkAccessManager: " <<  fi.absoluteFilePath() <<  " failed\n";
            DownloadOneFileFinish();
            return;
        }
        */
        m_qReply = m_qNetAccessManager->get(QNetworkRequest(QUrl(m_ListFile[m_nCurDLIndex].qstrUrl)));
        if (!m_qReply){
            m_qFile->close();
            m_qFile->remove();
            delete m_qFile;
            m_qFile = NULL;
            qDebug() << "=== call get: " <<  fi.absoluteFilePath() <<  " failed\n";
            //delete m_qNetAccessManager;
            //m_qNetAccessManager = NULL;
            DownloadOneFileFinish();
            return;
        }
        // Whenever more data is received from the network, this readyRead() signal is emitted
        connect(m_qReply, SIGNAL(readyRead()), this, SLOT(HttpReadyRead()));
        // This signal is emitted when the reply has finished processing.
        // After this signal is emitted, there will be no more updates to the reply's data or metadata.
        connect(m_qReply, SIGNAL(finished()), this, SLOT(HttpDownloadFinished()));
    }
}

void CDownloadFileMan::DownloadOneFileFinish(){
    QDateTime qDT;
    QFileInfo fi(m_ListFile[m_nCurDLIndex].qstrLocalFile);
    //struct timeval tv[2] = {{0,0}, {0,0}};
    struct utimbuf utb = {0,0};

    if (m_ListFile[m_nCurDLIndex].bResult){
        if (m_ListFile[m_nCurDLIndex].qullLMTime != 0){
            qDT = fi.lastRead();
            utb.actime  = qDT.toTime_t();
            utb.modtime = m_ListFile[m_nCurDLIndex].qullLMTime/1000;
            utime(m_ListFile[m_nCurDLIndex].qstrLocalFile.toStdString().c_str(), &utb);
        }

        //run update first time or new image is inserted in second time or after that
        if (m_bRunFirstTime || !m_ListImageLoaded.contains(m_ListFile[m_nCurDLIndex].nIdImage)){
            m_ListFile[m_nCurDLIndex].bSetCache = true;
            m_ListImageLoaded.push_back(m_ListFile[m_nCurDLIndex].nIdImage);
            if (!m_bRunFirstTime)
                qDebug() << "=== Image " << m_ListFile[m_nCurDLIndex].qstrLocalFile << " is added new ==> use cache";
            else {
                qDebug() << "=== Image " << m_ListFile[m_nCurDLIndex].qstrLocalFile << " is added first ==> use cache";
            }
        }else{//run update second time or after that
            m_ListFile[m_nCurDLIndex].bSetCache = false;
            qDebug() << "=== Image " << m_ListFile[m_nCurDLIndex].qstrLocalFile << " was modified ==> no use cache";
            //mark this image is modified, no never using from image-cache
            if (!m_ListImageModified.contains(m_ListFile[m_nCurDLIndex].nIdImage)){
                m_ListImageModified.push_back(m_ListFile[m_nCurDLIndex].nIdImage);
            }
        }
    }
    m_nCurDLIndex++;
    if (m_nCurDLIndex >= m_ListFile.count()){
        emit NotifyDownloadDone();
    }
    else{
        emit NotifyDownloadNextFile();
    }
}

void CDownloadFileMan::HttpReadyRead(){
    if (m_qFile)
        m_qFile->write(m_qReply->readAll());
}

// When download finished or canceled, this will be called
void CDownloadFileMan::HttpDownloadFinished(){
    if (m_qFile){
        m_qFile->flush();
        m_qFile->close();
    }
    // get redirection url
    QVariant redirectionTarget = m_qReply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (m_qReply->error()) {
        m_qFile->remove();
        m_ListFile[m_nCurDLIndex].bResult = false;
        //qDebug() << "Download file " + m_ListFile[m_nCurDLIndex].qstrUrl << " failed.\n";
        //DownloadOneFileFinish();
    } else if (!redirectionTarget.isNull()) {
        m_ListFile[m_nCurDLIndex].qstrUrl = redirectionTarget.toUrl().toString();
        m_qFile->remove();
        //qDebug() << "Download file " + m_ListFile[m_nCurDLIndex].qstrUrl << " redirect to " + redirectionTarget.toUrl().toString() << "\n";
        //emit NotifyDownloadNextFile();
    } else {
        m_ListFile[m_nCurDLIndex].bResult = true;
        //qDebug() << "Download file " + m_ListFile[m_nCurDLIndex].qstrUrl << " success\n";
        //DownloadOneFileFinish();
    }
    delete m_qFile;
    m_qFile = NULL;
    m_qReply->deleteLater();
    m_qReply = NULL;
//    m_qNetAccessManager->deleteLater();
//    m_qNetAccessManager = NULL;

    if (!redirectionTarget.isNull()){
        emit NotifyDownloadNextFile();
    }else{
        DownloadOneFileFinish();
    }
}


void CDownloadFileMan::DownloadDone(){
    if (m_qReply){
        delete m_qReply;
        m_qReply = NULL;
    }
    if (m_qFile){
        m_qFile->close();
        delete m_qFile;
        m_qFile = NULL;
    }
    if (m_qNetAccessManager){
        delete m_qNetAccessManager;
        m_qNetAccessManager = NULL;
    }
    m_bRunFirstTime = false;
    qDebug() << "Download complete\n";
    emit NotifyComplete(m_qstrData);
}

QList<DOWNFILE_INFO>& CDownloadFileMan::GetListFile(){
    return m_ListFile;
}

bool CDownloadFileMan::IsFileCached(){
    QFileInfo fi(m_ListFile[m_nCurDLIndex].qstrLocalFile);
    if (!fi.exists())
        return false;
    //qDebug() << "=== Check image cached: " << m_ListFile[m_nCurDLIndex].qstrLocalFile;
    QDateTime dtLMT = fi.lastModified();
    if ((m_ListFile[m_nCurDLIndex].qullLMTime/1000) == (dtLMT.toMSecsSinceEpoch()/1000)){
        //qDebug() << "=== File [" << m_ListFile[m_nCurDLIndex].qstrLocalFile << "] was cached";
        return true;
    }
    //qDebug() << "=== MFT json [" << m_ListFile[m_nCurDLIndex].qullLMTime << "] MFT file [" << dtLMT.toMSecsSinceEpoch() << "]";
    QFile(m_ListFile[m_nCurDLIndex].qstrLocalFile).remove();
    return false;
}

void CDownloadFileMan::SetData(const QString& qstrData){
    m_qstrData = qstrData;
}

QString CDownloadFileMan::GetData(){
    return m_qstrData;
}

void CDownloadFileMan::DeleteOldFiles(){
    QFileInfo fi(m_ListFile[m_nCurDLIndex].qstrLocalFile);
    QString qstrFilePath;

    QStringList nameFilters(fi.baseName() + ".*");
    QDirIterator dirIt(fi.absolutePath(), nameFilters, QDir::Files, QDirIterator::NoIteratorFlags);
    while (dirIt.hasNext()){
        qstrFilePath = dirIt.next();
        //qDebug() << qstrFilePath;
        QFile(qstrFilePath).remove();
    }
}

bool CDownloadFileMan::IsRunFirstTime(){
    return m_bRunFirstTime;
}
