/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtMultimedia module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QDebug>
#include <QCoreApplication>

#include <QtMultimedia/qaudioformat.h>
#include <QtMultimedia/qaudiodeviceinfo.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#include <alsa/asoundlib.h>
#include "n900audio.h"

#include "qdebug.h"

//#define DEBUG_AUDIO 1

N900AudioDeviceInfo::N900AudioDeviceInfo(QByteArray dev, QAudio::Mode mode)
{
    device = QLatin1String(dev);
    this->mode = mode;

    updateLists();
}

N900AudioDeviceInfo::~N900AudioDeviceInfo()
{
}

bool N900AudioDeviceInfo::isFormatSupported(const QAudioFormat& format) const
{
    return testSettings(format);
}

QAudioFormat N900AudioDeviceInfo::preferredFormat() const
{
    QAudioFormat nearest;
    if(mode == QAudio::AudioOutput) {
        nearest.setFrequency(44100);
        nearest.setChannels(2);
        nearest.setByteOrder(QAudioFormat::LittleEndian);
        nearest.setSampleType(QAudioFormat::SignedInt);
        nearest.setSampleSize(16);
        nearest.setCodec(QLatin1String("audio/pcm"));
    } else {
        nearest.setFrequency(8000);
        nearest.setChannels(1);
        nearest.setByteOrder(QAudioFormat::LittleEndian);
        nearest.setSampleType(QAudioFormat::UnSignedInt);
        nearest.setSampleSize(8);
        nearest.setCodec(QLatin1String("audio/pcm"));
    }
    return nearest;
}

QAudioFormat N900AudioDeviceInfo::nearestFormat(const QAudioFormat& format) const
{
    if(testSettings(format))
        return format;
    else
        return preferredFormat();
}

QString N900AudioDeviceInfo::deviceName() const
{
    return device;
}

QStringList N900AudioDeviceInfo::codecList()
{
    updateLists();
    return codecz;
}

QList<int> N900AudioDeviceInfo::frequencyList()
{
    updateLists();
    return freqz;
}

QList<int> N900AudioDeviceInfo::channelsList()
{
    updateLists();
    return channelz;
}

QList<int> N900AudioDeviceInfo::sampleSizeList()
{
    updateLists();
    return sizez;
}

QList<QAudioFormat::Endian> N900AudioDeviceInfo::byteOrderList()
{
    updateLists();
    return byteOrderz;
}

QList<QAudioFormat::SampleType> N900AudioDeviceInfo::sampleTypeList()
{
    updateLists();
    return typez;
}

bool N900AudioDeviceInfo::testSettings(const QAudioFormat& format) const
{
    if (!channelz.contains(format.channels()))
        return false;
    if (!codecz.contains(format.codec()))
        return false;
    if (!freqz.contains(format.frequency()))
        return false;
    if (!sizez.contains(format.sampleSize()))
        return false;
    if (!byteOrderz.contains(format.byteOrder()))
        return false;
    if (!typez.contains(format.sampleType()))
        return false;

    return true;
}

void N900AudioDeviceInfo::updateLists()
{
    freqz.clear();
    channelz.clear();
    sizez.clear();
    byteOrderz.clear();
    typez.clear();
    codecz.clear();

    if (mode == QAudio::AudioInput) {
        freqz.append(8000);
        channelz.append(1);
        sizez.append(8);
        sizez.append(16);
        byteOrderz.append(QAudioFormat::LittleEndian);
        typez.append(QAudioFormat::SignedInt);
        typez.append(QAudioFormat::UnSignedInt);
        codecz.append(QLatin1String("audio/pcm"));

    } else {
        for(int i=0; i<(int)MAX_SAMPLE_RATES; i++) {
            freqz.append(SAMPLE_RATES[i]);
        }
        channelz.append(2);
        sizez.append(8);
        sizez.append(16);
        byteOrderz.append(QAudioFormat::LittleEndian);
        typez.append(QAudioFormat::SignedInt);
        codecz.append(QLatin1String("audio/pcm"));
    }
}

QList<QByteArray> N900AudioDeviceInfo::availableDevices(QAudio::Mode mode)
{
    QList<QByteArray> devices;

    if(mode == QAudio::AudioInput)
        devices.append("pulseaudio");
    else
        devices.append("pulseaudio");

    return devices;
}

N900InputPrivate::N900InputPrivate(N900AudioInput* audio)
{
    audioDevice = audio;
}

N900InputPrivate::~N900InputPrivate()
{
}

qint64 N900InputPrivate::readData( char* data, qint64 len)
{
    return audioDevice->read(data,len);
}

qint64 N900InputPrivate::writeData(const char* data, qint64 len)
{
    Q_UNUSED(data)
    Q_UNUSED(len)
    return 0;
}

void N900InputPrivate::trigger()
{
    emit readyRead();
}

N900AudioInput::N900AudioInput(const QByteArray &device, const QAudioFormat& audioFormat):
    settings(audioFormat)
{
    bytesAvailable = 0;
    handle = 0;
    ahandler = 0;
    access = SND_PCM_ACCESS_RW_INTERLEAVED;
    pcmformat = SND_PCM_FORMAT_S16;
    buffer_size = 0;
    period_size = 0;
    buffer_time = 100000;
    period_time = 20000;
    totalTimeValue = 0;
    intervalTime = 1000;
    audioBuffer = 0;
    errorState = QAudio::NoError;
    deviceState = QAudio::StoppedState;
    audioSource = 0;
    pullMode = true;
    resuming = false;

    m_device = device;

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),SLOT(userFeed()));
}

N900AudioInput::~N900AudioInput()
{
    close();
    disconnect(timer, SIGNAL(timeout()));
    QCoreApplication::processEvents();
    delete timer;
}

int N900AudioInput::xrun_recovery(int err)
{
    int  count = 0;
    bool reset = false;

    if(err == -EPIPE) {
        errorState = QAudio::UnderrunError;
        err = snd_pcm_prepare(handle);
        if(err < 0)
            reset = true;
        else {
            bytesAvailable = bytesReady();
            if (bytesAvailable <= 0)
                reset = true;
        }

    } else if((err == -ESTRPIPE)||(err == -EIO)) {
        errorState = QAudio::IOError;
        while((err = snd_pcm_resume(handle)) == -EAGAIN){
            usleep(100);
            count++;
            if(count > 5) {
                reset = true;
                break;
            }
        }
        if(err < 0) {
            err = snd_pcm_prepare(handle);
            if(err < 0)
                reset = true;
        }
    }
    if(reset) {
        saveProcessed = totalTimeValue;
        close();
        open();
        totalTimeValue = saveProcessed;

        snd_pcm_prepare(handle);
        return 0;
    }
    return err;
}

int N900AudioInput::setFormat()
{
    snd_pcm_format_t format = SND_PCM_FORMAT_S16;

    if(settings.sampleSize() == 8) {
        format = SND_PCM_FORMAT_U8;
    } else if(settings.sampleSize() == 16) {
        if(settings.sampleType() == QAudioFormat::SignedInt) {
            if(settings.byteOrder() == QAudioFormat::LittleEndian)
                format = SND_PCM_FORMAT_S16_LE;
            else
                format = SND_PCM_FORMAT_S16_BE;
        } else if(settings.sampleType() == QAudioFormat::UnSignedInt) {
            if(settings.byteOrder() == QAudioFormat::LittleEndian)
                format = SND_PCM_FORMAT_U16_LE;
            else
                format = SND_PCM_FORMAT_U16_BE;
        }
    } else if(settings.sampleSize() == 24) {
        if(settings.sampleType() == QAudioFormat::SignedInt) {
            if(settings.byteOrder() == QAudioFormat::LittleEndian)
                format = SND_PCM_FORMAT_S24_LE;
            else
                format = SND_PCM_FORMAT_S24_BE;
        } else if(settings.sampleType() == QAudioFormat::UnSignedInt) {
            if(settings.byteOrder() == QAudioFormat::LittleEndian)
                format = SND_PCM_FORMAT_U24_LE;
            else
                format = SND_PCM_FORMAT_U24_BE;
        }
    } else if(settings.sampleSize() == 32) {
        if(settings.sampleType() == QAudioFormat::SignedInt) {
            if(settings.byteOrder() == QAudioFormat::LittleEndian)
                format = SND_PCM_FORMAT_S32_LE;
            else
                format = SND_PCM_FORMAT_S32_BE;
        } else if(settings.sampleType() == QAudioFormat::UnSignedInt) {
            if(settings.byteOrder() == QAudioFormat::LittleEndian)
                format = SND_PCM_FORMAT_U32_LE;
            else
                format = SND_PCM_FORMAT_U32_BE;
        } else if(settings.sampleType() == QAudioFormat::Float) {
            if(settings.byteOrder() == QAudioFormat::LittleEndian)
                format = SND_PCM_FORMAT_FLOAT_LE;
            else
                format = SND_PCM_FORMAT_FLOAT_BE;
        }
    } else if(settings.sampleSize() == 64) {
        if(settings.byteOrder() == QAudioFormat::LittleEndian)
            format = SND_PCM_FORMAT_FLOAT64_LE;
        else
            format = SND_PCM_FORMAT_FLOAT64_BE;
    }

    return snd_pcm_hw_params_set_format( handle, hwparams, format);
}

void N900AudioInput::close()
{
    deviceState = QAudio::StoppedState;
    timer->stop();

    if ( handle ) {
        snd_pcm_drop( handle );
        snd_pcm_close( handle );
        handle = 0;
        delete [] audioBuffer;
        audioBuffer=0;
    }
}

void N900AudioInput::userFeed()
{
    if(deviceState == QAudio::StoppedState || deviceState == QAudio::SuspendedState)
        return;
#ifdef DEBUG_AUDIO
    QTime now(QTime::currentTime());
    qDebug()<<now.second()<<"s "<<now.msec()<<"ms :userFeed() IN";
#endif
    if(pullMode) {
        // reads some audio data and writes it to QIODevice
        read(0,0);
    } else {
        // emits readyRead() so user will call read() on QIODevice to get some audio data
        N900InputPrivate* a = qobject_cast<N900InputPrivate*>(audioSource);
        a->trigger();
    }
    bytesAvailable = bytesReady();

    if(deviceState != QAudio::ActiveState)
        return;

    if((timeStamp.elapsed() + elapsedTimeOffset)> intervalTime) {
        emit notify();
        elapsedTimeOffset = timeStamp.elapsed() + elapsedTimeOffset - intervalTime;
        timeStamp.restart();
    }
}

bool N900AudioInput::setParams()
{
    int dir;
    int err;
    unsigned int rate;
    unsigned int freakuency=settings.frequency();

    snd_pcm_hw_params_alloca(&hwparams);
    snd_pcm_sw_params_alloca(&swparams);
    err = snd_pcm_hw_params_any(handle, hwparams);
    if (err < 0) {
        qWarning()<<"Broken configuration for this PCM: no configurations available";
        return false;
    }
    err = snd_pcm_hw_params_set_rate_resample( handle, hwparams, 1 );

    err = snd_pcm_hw_params_set_access(handle, hwparams,access);
    if (err < 0) {
        qWarning()<<"access not available";
        return false;
    }
    err = setFormat();
    if (err < 0) {
        qWarning()<<"format not available";
        return false;
    }
    err = snd_pcm_hw_params_set_channels(handle, hwparams, settings.channels());
    if (err < 0) {
        qWarning()<<"channels mode "<<settings.channels()<<" not available";
        return false;
    }
    rate = settings.frequency();
    err = snd_pcm_hw_params_set_rate_near(handle, hwparams, &freakuency, 0);
    if (err < 0) {
        qWarning()<<"Warning: rate is not accurate: want "<<rate<<", got "<<freakuency;
    }
    err = snd_pcm_hw_params_set_buffer_time_near(handle, hwparams, &buffer_time, 0);
    err = snd_pcm_hw_params_set_period_time_near(handle, hwparams, &period_time, 0);
    unsigned int chunks = 8;
    err = snd_pcm_hw_params_set_periods_near(handle, hwparams, &chunks, 0);
    err = snd_pcm_hw_params(handle, hwparams);
    if (err < 0) {
        qWarning()<<"cant set hw params";
        return false;
    }
    snd_pcm_hw_params_get_buffer_size(hwparams,&buffer_frames);
    buffer_size = snd_pcm_frames_to_bytes(handle,buffer_frames);
    snd_pcm_hw_params_get_period_size(hwparams,&period_frames, &dir);
    period_size = snd_pcm_frames_to_bytes(handle,period_frames);
    snd_pcm_hw_params_get_buffer_time(hwparams,&buffer_time, &dir);
    snd_pcm_hw_params_get_period_time(hwparams,&period_time, &dir);

    snd_pcm_sw_params_current(handle, swparams);
    snd_pcm_sw_params_set_start_threshold(handle,swparams,period_frames);
    snd_pcm_sw_params_set_stop_threshold(handle,swparams,buffer_frames);
    snd_pcm_sw_params_set_avail_min(handle, swparams,period_frames);
    snd_pcm_sw_params(handle, swparams);

    if (err < 0) {
        qWarning()<<"cant set sw params";
        return false;
    }
    return true;
}

bool N900AudioInput::open()
{
#ifdef DEBUG_AUDIO
    QTime now(QTime::currentTime());
    qDebug()<<now.second()<<"s "<<now.msec()<<"ms :open()";
#endif
    clockStamp.restart();
    timeStamp.restart();
    elapsedTimeOffset = 0;

    int err=-1;
    int count=0;

    QString dev = QLatin1String("plughw:0,0");

    // Step 1: try and open the device
    while((count < 5) && (err < 0)) {
        err=snd_pcm_open(&handle,dev.toLocal8Bit().constData(),SND_PCM_STREAM_CAPTURE,0);
        if(err < 0)
            count++;
    }
    if (( err < 0)||(handle == 0)) {
        qWarning()<<"Unable to open input device "<<dev;
        errorState = QAudio::OpenError;
        deviceState = QAudio::StoppedState;
        emit stateChanged(deviceState);
        return false;
    }
    snd_pcm_nonblock( handle, 0 );

    if(!setParams()) {
        errorState = QAudio::OpenError;
        deviceState = QAudio::StoppedState;
        emit stateChanged(deviceState);
        return false;
    }

    // Step 4: Prepare audio
    if(audioBuffer == 0)
        audioBuffer = new char[buffer_size];
    snd_pcm_prepare( handle );
    snd_pcm_start(handle);

    // Step 5: Setup timer
    bytesAvailable = bytesReady();

    if(pullMode)
        connect(audioSource,SIGNAL(readyRead()),this,SLOT(userFeed()));

    // Step 6: Start audio processing
    int chunks = buffer_size/period_size;
    timer->start(period_time*chunks/2000);

    errorState  = QAudio::NoError;

    totalTimeValue = 0;

    return true;
}

qint64 N900AudioInput::read(char* data, qint64 len)
{
    Q_UNUSED(len)

    // Read in some audio data and write it to QIODevice, pull mode
    if ( !handle )
        return 0;

    bytesAvailable = bytesReady();

    if (bytesAvailable < 0) {
        // bytesAvailable as negative is error code, try to recover from it.
        xrun_recovery(bytesAvailable);
        bytesAvailable = bytesReady();
        if (bytesAvailable < 0) {
            // recovery failed must stop and set error.
            close();
            errorState = QAudio::IOError;
            deviceState = QAudio::StoppedState;
            emit stateChanged(deviceState);
            return 0;
        }
    }

    int count=0, err = 0;
    while(count < 5) {
        int chunks = bytesAvailable/period_size;
        int frames = chunks*period_frames;
        if(frames > (int)buffer_frames)
            frames = buffer_frames;
        int readFrames = snd_pcm_readi(handle, audioBuffer, frames);
        if (readFrames >= 0) {
            err = snd_pcm_frames_to_bytes(handle, readFrames);
#ifdef DEBUG_AUDIO
            qDebug()<<QString::fromLatin1("read in bytes = %1 (frames=%2)").arg(err).arg(readFrames).toLatin1().constData();
#endif
            break;
        } else if((readFrames == -EAGAIN) || (readFrames == -EINTR)) {
            errorState = QAudio::IOError;
            err = 0;
            break;
        } else {
            if(readFrames == -EPIPE) {
                errorState = QAudio::UnderrunError;
                err = snd_pcm_prepare(handle);
            } else if(readFrames == -ESTRPIPE) {
                err = snd_pcm_prepare(handle);
            }
            if(err != 0) break;
        }
        count++;
    }
    if(err > 0) {
        // got some send it onward
#ifdef DEBUG_AUDIO
        qDebug()<<"frames to write to QIODevice = "<<
            snd_pcm_bytes_to_frames( handle, (int)err )<<" ("<<err<<") bytes";
#endif
        if(deviceState != QAudio::ActiveState && deviceState != QAudio::IdleState)
            return 0;
        if (pullMode) {
            qint64 l = audioSource->write(audioBuffer,err);
            if(l < 0) {
                close();
                errorState = QAudio::IOError;
                deviceState = QAudio::StoppedState;
                emit stateChanged(deviceState);
            } else if(l == 0) {
                if (deviceState != QAudio::IdleState) {
                    errorState = QAudio::NoError;
                    deviceState = QAudio::IdleState;
                    emit stateChanged(deviceState);
                }
            } else {
                totalTimeValue += l;
                resuming = false;
                if (deviceState != QAudio::ActiveState) {
                    errorState = QAudio::NoError;
                    deviceState = QAudio::ActiveState;
                    emit stateChanged(deviceState);
                }
            }
            return l;

        } else {
            memcpy(data,audioBuffer,err);
            totalTimeValue += err;
            resuming = false;
            if (deviceState != QAudio::ActiveState) {
                errorState = QAudio::NoError;
                deviceState = QAudio::ActiveState;
                emit stateChanged(deviceState);
            }
            return err;
        }
    }
    return 0;
}

QIODevice* N900AudioInput::start(QIODevice* device)
{
    if(deviceState != QAudio::StoppedState)
        close();

    if(!pullMode && audioSource) {
        delete audioSource;
    }

    if(device) {
        //set to pull mode
        pullMode = true;
        audioSource = device;
        deviceState = QAudio::ActiveState;
    } else {
        //set to push mode
        pullMode = false;
        deviceState = QAudio::IdleState;
        audioSource = new N900InputPrivate(this);
        audioSource->open(QIODevice::ReadWrite | QIODevice::Unbuffered);
    }

    if( !open() )
        return 0;

    emit stateChanged(deviceState);

    return audioSource;
}

void N900AudioInput::stop()
{
    if(deviceState == QAudio::StoppedState)
        return;

    deviceState = QAudio::StoppedState;
    errorState = QAudio::NoError;

    close();
    emit stateChanged(deviceState);
}

void N900AudioInput::reset()
{
}

void N900AudioInput::suspend()
{
    if(deviceState == QAudio::ActiveState||deviceState == QAudio::IdleState||resuming) {
        saveProcessed = totalTimeValue;
        timer->stop();
        deviceState = QAudio::SuspendedState;
        emit stateChanged(deviceState);
    }
}

void N900AudioInput::resume()
{
    if(deviceState == QAudio::SuspendedState) {
        int err = 0;

        if(handle) {
            err = snd_pcm_prepare( handle );
            if(err < 0)
                xrun_recovery(err);

            err = snd_pcm_start(handle);
            if(err < 0)
                xrun_recovery(err);

            bytesAvailable = buffer_size;
        }
        resuming = true;
        totalTimeValue = saveProcessed;
        deviceState = QAudio::ActiveState;
        int chunks = buffer_size/period_size;
        timer->start(period_time*chunks/2000);
        emit stateChanged(deviceState);
    }
}

int N900AudioInput::bytesReady() const
{
    if(resuming)
        return period_size;

    if(deviceState != QAudio::ActiveState && deviceState != QAudio::IdleState)
        return 0;
    int frames = snd_pcm_avail_update(handle);
    if((int)frames > (int)buffer_frames || (int)frames < 0)
        frames = buffer_frames;

    return snd_pcm_frames_to_bytes(handle, frames);
}

int N900AudioInput::periodSize() const
{
    return period_size;
}

void N900AudioInput::setBufferSize(int value)
{
    buffer_size = value;
}

int N900AudioInput::bufferSize() const
{
    return buffer_size;
}

void N900AudioInput::setNotifyInterval(int ms)
{
    if (ms <= 0)
        intervalTime = 0;
    else
        intervalTime = ms;
}

int N900AudioInput::notifyInterval() const
{
    return intervalTime;
}

qint64 N900AudioInput::processedUSecs() const
{
    qint64 result = qint64(1000000) * totalTimeValue /
        (settings.channels()*(settings.sampleSize()/8)) /
        settings.frequency();

    return result;
}

qint64 N900AudioInput::elapsedUSecs() const
{
    if (deviceState == QAudio::StoppedState)
        return 0;

    return clockStamp.elapsed()*1000;
}

QAudio::Error N900AudioInput::error() const
{
    return errorState;
}

QAudio::State N900AudioInput::state() const
{
    return deviceState;
}

QAudioFormat N900AudioInput::format() const
{
    return settings;
}

N900OutputPrivate::N900OutputPrivate(N900AudioOutput* audio)
{
    audioDevice = audio;
}

N900OutputPrivate::~N900OutputPrivate() {}

qint64 N900OutputPrivate::readData( char* data, qint64 len)
{
    Q_UNUSED(data)
    Q_UNUSED(len)

    return 0;
}

qint64 N900OutputPrivate::writeData(const char* data, qint64 len)
{
    int retry = 0;
    qint64 written = 0;

    if((audioDevice->state() == QAudio::ActiveState)
            ||(audioDevice->state() == QAudio::IdleState)) {
        while(written < len) {
            int chunk = audioDevice->write(data+written,(len-written));
            if(chunk <= 0)
                retry++;
            written+=chunk;
            if(retry > 10)
                return written;
        }
    }
    return written;
}

N900AudioOutput::N900AudioOutput(const QByteArray &device, const QAudioFormat& audioFormat):
    settings(audioFormat)
{
    bytesAvailable = 0;
    buffer_size = 0;
    period_size = 0;
    buffer_time = 100000;
    period_time = 20000;
    totalTimeValue = 0;
    intervalTime = 1000;
    audioBuffer = 0;
    errorState = QAudio::NoError;
    deviceState = QAudio::StoppedState;
    audioSource = 0;
    pullMode = true;
    handle = 0;

    dummyBuffer = 0;

    m_device = device;

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),SLOT(userFeed()));
}

N900AudioOutput::~N900AudioOutput()
{
    close();
    disconnect(timer, SIGNAL(timeout()));
    QCoreApplication::processEvents();
    delete timer;
}

qint64 N900AudioOutput::write(const char *data, qint64 len )
{
    qint64 length = len;

    if(!connected)
        return 0;

    writing = true;

    if (length <= 0 || dummyBuffer <= 0) return 0;
    if (length > buffer_size) length = buffer_size;
    if (dummyBuffer-length < 0) length = dummyBuffer;

    if (pa_simple_write(handle, data, (size_t)length, &err) < 0) {
        qWarning()<<"QAudioOutput::write err, can't write to pulseaudio daemon:"<<pa_strerror(err);
        close();
        connected = false;
        errorState = QAudio::OpenError;
        deviceState = QAudio::StoppedState;
        emit stateChanged(deviceState);
        return 0;
    } else {
        writeTime.restart();
        totalTimeValue += length;
        dummyBuffer -= length;
        errorState = QAudio::NoError;
        if (deviceState != QAudio::ActiveState) {
            deviceState = QAudio::ActiveState;
            emit stateChanged(deviceState);
        }
        return length;
    }
    return 0;
}

bool N900AudioOutput::open()
{
    QTime now(QTime::currentTime());

    clockTime.restart();
    timeStamp.restart();
    writeTime.restart();

    count     = 0;

    params.format = PA_SAMPLE_S16LE;

    if(settings.sampleType() == QAudioFormat::SignedInt) {
        if(settings.sampleSize() == 8) {
            qWarning()<<"unsupported format";
            errorState = QAudio::OpenError;
            deviceState = QAudio::StoppedState;
            emit stateChanged(deviceState);
            return false;

        } else if(settings.sampleSize() == 16) {
            if(settings.byteOrder() == QAudioFormat::LittleEndian)
                params.format = PA_SAMPLE_S16LE;
            else
                params.format = PA_SAMPLE_S16BE;
        }

    } else if(settings.sampleType() == QAudioFormat::UnSignedInt) {
        if(settings.sampleSize() == 8) {
            params.format = PA_SAMPLE_U8;
        } else if(settings.sampleSize() == 16) {
            qWarning()<<"unsupported format";
            errorState = QAudio::OpenError;
            deviceState = QAudio::StoppedState;
            emit stateChanged(deviceState);
            return false;
        }

    } else {
        if(settings.sampleSize() == 8) {
            params.format = PA_SAMPLE_U8;
        } else {
            qWarning()<<"unsupported format";
            errorState = QAudio::OpenError;
            deviceState = QAudio::StoppedState;
            emit stateChanged(deviceState);
            return false;
        }
    }
    params.rate = settings.frequency();
    params.channels = settings.channels();

    memset(&attr,0,sizeof(attr));
    attr.tlength = pa_bytes_per_second(&params)/6;
    attr.maxlength = (attr.tlength*3)/2;
    attr.minreq = attr.tlength/50;
    attr.prebuf = (attr.tlength - attr.minreq)/4;
    attr.fragsize = attr.tlength/50;
    buffer_size = attr.tlength*3;
    period_size = buffer_size/5;

    if(!(handle = pa_simple_new(NULL, m_device.constData(),
                    PA_STREAM_PLAYBACK, NULL,
                    QString("pulseaudio:%1").arg(::getpid()).toAscii().constData(),
                    &params, NULL, &attr, &err))) {
        qWarning()<<"QAudioOutput failed to open, your pulseaudio daemon is not configured correctly";
        errorState = QAudio::OpenError;
        deviceState = QAudio::StoppedState;
        emit stateChanged(deviceState);
        return false;
    }

    connected = true;
    writing   = false;

    if(audioBuffer == 0)
        audioBuffer = new char[buffer_size];

    if(pullMode)
        connect(audioSource,SIGNAL(readyRead()),this,SLOT(userFeed()));

    timer->start(20);

    errorState  = QAudio::NoError;

    totalTimeValue = 0;
    dummyBuffer = buffer_size;

    return true;
}

void N900AudioOutput::userFeed()
{
    if(deviceState == QAudio::StoppedState || deviceState == QAudio::SuspendedState)
        return;

    if(pullMode) {
        // write some audio data and writes it to QIODevice
        while (dummyBuffer >= period_size) {
            int l = audioSource->read(audioBuffer,period_size);
            if(l > 0) {
                qint64 bytesWritten = write(audioBuffer,l);
                if (bytesWritten != l) {
                    audioSource->seek(audioSource->pos()-(l-bytesWritten));
                    break;
                }

            } else if(l == 0) {
                if (deviceState != QAudio::IdleState) {
                    errorState = QAudio::UnderrunError;
                    deviceState = QAudio::IdleState;
                    emit stateChanged(deviceState);
                }
                break;

            } else {
                close();
                errorState = QAudio::IOError;
                emit stateChanged(deviceState);
                break;

            }
        }
    } else {
        if (writeTime.elapsed() > 40 && deviceState != QAudio::IdleState) {
            deviceState = QAudio::IdleState;
            errorState = QAudio::UnderrunError;
            emit stateChanged(deviceState);
        }
    }

    if(intervalTime > 0 && timeStamp.elapsed() > intervalTime) {
        emit notify();
        timeStamp.restart();
    }
    dummyBuffer+=period_size;
    if (dummyBuffer > buffer_size) dummyBuffer = buffer_size;
}

void N900AudioOutput::close()
{
    deviceState = QAudio::StoppedState;
    timer->stop();

    if(handle) {
        pa_simple_drain(handle, &err);
        pa_simple_free(handle);
        handle = 0;
        dummyBuffer = buffer_size;
    }
}

QIODevice* N900AudioOutput::start(QIODevice* device)
{
    if(deviceState != QAudio::StoppedState)
        deviceState = QAudio::StoppedState;

    errorState = QAudio::NoError;

    // Handle change of mode
    if(audioSource && pullMode && !device) {
        // pull -> push
        close();
        audioSource = 0;
    } else if(audioSource && !pullMode && device) {
        // push -> pull
        close();
        delete audioSource;
        audioSource = 0;
    }

    if(device) {
        //set to pull mode
        pullMode = true;
        audioSource = device;
        deviceState = QAudio::ActiveState;
    } else {
        //set to push mode
        if(!audioSource) {
            audioSource = new N900OutputPrivate(this);
            audioSource->open(QIODevice::WriteOnly|QIODevice::Unbuffered);
        }
        pullMode = false;
        deviceState = QAudio::IdleState;
    }

    if (!open())
        return 0;

    emit stateChanged(deviceState);

    return audioSource;
}

void N900AudioOutput::stop()
{
    if(deviceState == QAudio::StoppedState)
        return;
    errorState = QAudio::NoError;
    close();
    emit stateChanged(deviceState);
}

void N900AudioOutput::reset()
{
}

void N900AudioOutput::suspend()
{
    if(deviceState == QAudio::ActiveState || deviceState == QAudio::IdleState) {
        timer->stop();
        saveProcessed = totalTimeValue;
        close();
        deviceState = QAudio::SuspendedState;
        errorState = QAudio::NoError;
        emit stateChanged(deviceState);
    }
}

void N900AudioOutput::resume()
{
    if(deviceState == QAudio::SuspendedState) {
        deviceState = QAudio::ActiveState;
        if (!open()) return;
        totalTimeValue = saveProcessed;
        emit stateChanged(deviceState);
    }
}

int N900AudioOutput::bytesFree() const
{
    if(deviceState != QAudio::ActiveState && deviceState != QAudio::IdleState)
        return 0;
    return dummyBuffer;
}

int N900AudioOutput::periodSize() const
{
    return period_size;
}

void N900AudioOutput::setBufferSize(int value)
{
    buffer_size = value;
}

int N900AudioOutput::bufferSize() const
{
    return buffer_size;
}

void N900AudioOutput::setNotifyInterval(int ms)
{
    if (ms <= 0)
        intervalTime = 0;
    else
        intervalTime = ms;
}

int N900AudioOutput::notifyInterval() const
{
    return intervalTime;
}

qint64 N900AudioOutput::processedUSecs() const
{
    if (deviceState == QAudio::StoppedState)
        return 0;
    qint64 result = qint64(1000000) * totalTimeValue /
        (settings.channels()*(settings.sampleSize()/8)) /
        settings.frequency();

    return result;
}

qint64 N900AudioOutput::elapsedUSecs() const
{
    if(deviceState == QAudio::StoppedState)
        return 0;

    return clockTime.elapsed()*1000;
}

QAudio::Error N900AudioOutput::error() const
{
    return errorState;
}

QAudio::State N900AudioOutput::state() const
{
    return deviceState;
}

QAudioFormat N900AudioOutput::format() const
{
    return settings;
}
