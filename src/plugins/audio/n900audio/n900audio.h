/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the plugins of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the either Technology Preview License Agreement or the
** Beta Release License Agreement.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef N900AUDIO_H
#define N900AUDIO_H

#include <QObject>
#include <QTime>
#include <QTimer>
#include <QByteArray>
#include <QIODevice>
#include <QtMultimedia/qaudioformat.h>
#include <QtMultimedia/qaudiodeviceinfo.h>
#include <QtMultimedia/qaudioengine.h>

#include <alsa/asoundlib.h>
#include <pulse/simple.h>
#include <pulse/error.h>

const unsigned int MAX_SAMPLE_RATES = 5;
const unsigned int SAMPLE_RATES[] =
    { 8000, 11025, 22050, 44100, 48000 };

class N900AudioDeviceInfo : public QAbstractAudioDeviceInfo
{
    Q_OBJECT
public:
    N900AudioDeviceInfo(QByteArray dev, QAudio::Mode mode);
    ~N900AudioDeviceInfo();

    bool testSettings(const QAudioFormat& format) const;
    void updateLists();
    QAudioFormat preferredFormat() const;
    bool isFormatSupported(const QAudioFormat& format) const;
    QAudioFormat nearestFormat(const QAudioFormat& format) const;
    QString deviceName() const;
    QStringList codecList();
    QList<int> frequencyList();
    QList<int> channelsList();
    QList<int> sampleSizeList();
    QList<QAudioFormat::Endian> byteOrderList();
    QList<QAudioFormat::SampleType> sampleTypeList();
    QList<QByteArray> availableDevices(QAudio::Mode);

private:
    QString device;
    QAudio::Mode mode;
    QAudioFormat settings;
    QAudioFormat nearest;
    QList<int> freqz;
    QList<int> channelz;
    QList<int> sizez;
    QList<QAudioFormat::Endian> byteOrderz;
    QStringList codecz;
    QList<QAudioFormat::SampleType> typez;
    int fd;
};

class N900AudioInput;

class N900InputPrivate : public QIODevice
{
    friend class N900AudioInput;
    Q_OBJECT
public:
    N900InputPrivate(N900AudioInput* audio);
    ~N900InputPrivate();

    qint64 readData( char* data, qint64 len);
    qint64 writeData(const char* data, qint64 len);

    void trigger();
private:
    N900AudioInput *audioDevice;
};

class N900AudioInput : public QAbstractAudioInput
{
    friend class N900InputPrivate;
    Q_OBJECT
public:
    N900AudioInput(const QByteArray &device, const QAudioFormat& audioFormat);
    ~N900AudioInput();

    qint64 read(char* data, qint64 len);
    QIODevice* start(QIODevice* device = 0);
    void stop();
    void reset();
    void suspend();
    void resume();
    int bytesReady() const;
    int periodSize() const;
    void setBufferSize(int value);
    int bufferSize() const;
    void setNotifyInterval(int milliSeconds);
    int notifyInterval() const;
    qint64 processedUSecs() const;
    qint64 elapsedUSecs() const;
    QAudio::Error error() const;
    QAudio::State state() const;
    QAudioFormat format() const;

    bool resuming;
    snd_pcm_t* handle;

private slots:
    void userFeed();

private:
    int xrun_recovery(int err);
    bool setParams();
    int setFormat();
    bool open();
    void close();
    void drain();

    QByteArray m_device;
    QAudioFormat settings;
    QAudio::Error errorState;
    QAudio::State deviceState;
    QIODevice* audioSource;
    bool pullMode;
    QTimer* timer;
    QTime timeStamp;
    QTime clockTime;
    int intervalTime;
    char* audioBuffer;
    int bytesAvailable;
    int buffer_size;
    int period_size;
    unsigned int buffer_time;
    unsigned int period_time;
    qint64 totalTimeValue;
    QTime clockStamp;
    qint64 elapsedTimeOffset;
    qint64 saveProcessed;

    snd_pcm_uframes_t buffer_frames;
    snd_pcm_uframes_t period_frames;
    snd_async_handler_t* ahandler;
    snd_pcm_access_t access;
    snd_pcm_format_t pcmformat;
    snd_timestamp_t* timestamp;
    snd_pcm_hw_params_t *hwparams;
    snd_pcm_sw_params_t *swparams;
};

class N900AudioOutput;

class N900OutputPrivate : public QIODevice
{
    Q_OBJECT
public:
    N900OutputPrivate(N900AudioOutput* audio);
    ~N900OutputPrivate();

    qint64 readData( char* data, qint64 len);
    qint64 writeData(const char* data, qint64 len);

private:
    N900AudioOutput *audioDevice;
};

class N900AudioOutput : public QAbstractAudioOutput
{
    friend class N900OutputPrivate;
    Q_OBJECT
public:
    N900AudioOutput(const QByteArray &device, const QAudioFormat& audioFormat);
    ~N900AudioOutput();

    qint64 write( const char *data, qint64 len );
    QIODevice* start(QIODevice* device = 0);
    void stop();
    void reset();
    void suspend();
    void resume();
    int bytesFree() const;
    int periodSize() const;
    void setBufferSize(int value);
    int bufferSize() const;
    void setNotifyInterval(int milliSeconds);
    int notifyInterval() const;
    qint64 processedUSecs() const;
    qint64 elapsedUSecs() const;
    QAudio::Error error() const;
    QAudio::State state() const;
    QAudioFormat format() const;

private slots:
    void userFeed();

private:
    bool open();
    void close();

    QByteArray m_device;
    QAudioFormat settings;
    QAudio::Error errorState;
    QAudio::State deviceState;
    QIODevice* audioSource;
    bool pullMode;
    QTimer* timer;
    QTime timeStamp;
    QTime clockTime;
    QTime writeTime;
    int intervalTime;
    char* audioBuffer;
    int bytesAvailable;
    int buffer_size;
    int period_size;
    unsigned int buffer_time;
    unsigned int period_time;
    qint64 totalTimeValue;
    qint64 saveProcessed;

    pa_sample_spec  params;
    pa_simple*      handle;
    pa_buffer_attr  attr;
    bool            connected;
    bool            writing;
    int             count;
    int             err;

    int dummyBuffer;
};

#endif
