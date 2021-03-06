/*
 * Copyright (C) 2016 Stuart Howarth <showarth@marxoft.co.uk>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PLUGINVIDEO_H
#define PLUGINVIDEO_H

#include "video.h"
#include "resourcesrequest.h"

class PluginVideo : public CTVideo
{
    Q_OBJECT
    
    Q_PROPERTY(QString commentsId READ commentsId NOTIFY commentsIdChanged)
    Q_PROPERTY(QString relatedVideosId READ relatedVideosId NOTIFY relatedVideosIdChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY statusChanged)
    Q_PROPERTY(ResourcesRequest::Status status READ status NOTIFY statusChanged)

public:
    explicit PluginVideo(QObject *parent = 0);
    explicit PluginVideo(const QString &service, const QString &id, QObject *parent = 0);
    explicit PluginVideo(const QString &service, const QVariantMap &video, QObject *parent = 0);
    explicit PluginVideo(const PluginVideo *video, QObject *parent = 0);
    
    QString commentsId() const;
    QString relatedVideosId() const;
    
    QString errorString() const;
        
    ResourcesRequest::Status status() const;
    
    Q_INVOKABLE void loadVideo(const QString &service, const QString &id);
    Q_INVOKABLE void loadVideo(const QString &service, const QVariantMap &video);
    Q_INVOKABLE void loadVideo(PluginVideo *video);

protected:
    void setCommentsId(const QString &i);
    void setRelatedVideosId(const QString &i);

private Q_SLOTS:
    void onRequestFinished();
    
Q_SIGNALS:
    void commentsIdChanged();
    void relatedVideosIdChanged();
    void statusChanged(ResourcesRequest::Status s);

private:
    ResourcesRequest* request();
    
    ResourcesRequest *m_request;
    
    QString m_commentsId;
    QString m_relatedVideosId;
};

#endif // PLUGINVIDEO_H
