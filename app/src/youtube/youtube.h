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

#ifndef YOUTUBE_H
#define YOUTUBE_H

#include <QObject>
#include <QStringList>
#include <QVariantMap>
#include <QRegExp>
#include <QUrl>

class YouTubeComment;
class YouTubePlaylist;
class YouTubeUser;
class YouTubeVideo;

class YouTube : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString userId READ userId WRITE setUserId NOTIFY userIdChanged)
    Q_PROPERTY(QString accessToken READ accessToken WRITE setAccessToken NOTIFY accessTokenChanged)
    Q_PROPERTY(QString refreshToken READ refreshToken WRITE setRefreshToken NOTIFY refreshTokenChanged)
    Q_PROPERTY(QVariantMap relatedPlaylists READ relatedPlaylists WRITE setRelatedPlaylists
               NOTIFY relatedPlaylistsChanged)
    Q_PROPERTY(QString apiKey READ apiKey WRITE setApiKey NOTIFY apiKeyChanged)
    Q_PROPERTY(QString clientId READ clientId WRITE setClientId NOTIFY clientIdChanged)
    Q_PROPERTY(QString clientSecret READ clientSecret WRITE setClientSecret NOTIFY clientSecretChanged)
    Q_PROPERTY(QStringList scopes READ scopes WRITE setScopes NOTIFY scopesChanged)
    Q_PROPERTY(QString AUDIT_SCOPE READ auditScope CONSTANT)
    Q_PROPERTY(QString FORCE_SSL_SCOPE READ forceSslScope CONSTANT)
    Q_PROPERTY(QString PARTNER_SCOPE READ partnerScope CONSTANT)
    Q_PROPERTY(QString READ_ONLY_SCOPE READ readOnlyScope CONSTANT)
    Q_PROPERTY(QString READ_WRITE_SCOPE READ readWriteScope CONSTANT)
    Q_PROPERTY(QString UPLOAD_SCOPE READ uploadScope CONSTANT)
            
public:
    ~YouTube();
    
    static const QRegExp URL_REGEXP;
    
    static YouTube* instance();
        
    Q_INVOKABLE static QString formatDuration(const QString &duration);
    
    Q_INVOKABLE static QString getErrorString(const QVariantMap &error);
    
    Q_INVOKABLE static QString getPlaylistId(const QVariantMap &playlist);
    Q_INVOKABLE static QString getUserId(const QVariantMap &user);
    Q_INVOKABLE static QString getVideoId(const QVariantMap &video);
    
    Q_INVOKABLE static QString relatedPlaylist(const QString &name);
    
    Q_INVOKABLE static QUrl authUrl();
    
    static QString userId();
        
    static QString accessToken();
    
    static QString refreshToken();
        
    static QVariantMap relatedPlaylists();
    
    static QString apiKey();
    
    static QString clientId();
    
    static QString clientSecret();
    
    static QStringList scopes();
    
    Q_INVOKABLE static bool hasScope(const QString &scope);
    
    static QString auditScope();
    static QString forceSslScope();
    static QString partnerScope();
    static QString readOnlyScope();
    static QString readWriteScope();
    static QString uploadScope();
    
public Q_SLOTS:
    static void init();
    
    static void setUserId(const QString &id);
    
    static void setAccessToken(const QString &token);
    
    static void setRefreshToken(const QString &token);
        
    static void setRelatedPlaylists(const QVariantMap &playlists);
                
    static void setApiKey(const QString &key);
    
    static void setClientId(const QString &id);
    
    static void setClientSecret(const QString &secret);
    
    static void setScopes(const QStringList &s);
    
Q_SIGNALS:
    void userIdChanged(const QString &id);
    void accessTokenChanged(const QString &token);
    void refreshTokenChanged(const QString &token);
    void relatedPlaylistsChanged(const QVariantMap &playlists);
    void apiKeyChanged(const QString &key);
    void clientIdChanged(const QString &key);
    void clientSecretChanged(const QString &secret);
    void scopesChanged(const QStringList &scopes);
    
    void commentAdded(YouTubeComment *comment);
    
    void playlistCreated(YouTubePlaylist *playlist);
    void playlistDeleted(YouTubePlaylist *playlist);
    
    void userSubscribed(YouTubeUser *user);
    void userUnsubscribed(YouTubeUser *user);
    
    void videoAddedToPlaylist(YouTubeVideo *video, YouTubePlaylist *playlist);
    void videoRemovedFromPlaylist(YouTubeVideo *video, YouTubePlaylist *playlist);
    void videoDisliked(YouTubeVideo *video);
    void videoLiked(YouTubeVideo *video);
    void videoFavourited(YouTubeVideo *video);
    void videoUnfavourited(YouTubeVideo *video);

private:
    YouTube();
    
    struct SubscriptionCache {
        QHash<QString, QString> ids;
        QString nextPageToken;
        bool loaded;

        SubscriptionCache() :
            loaded(false)
        {
        }
    };
    
    static SubscriptionCache subscriptionCache;
    static YouTube *self;
    
    static const QString API_KEY;
    static const QString CLIENT_ID;
    static const QString CLIENT_SECRET;
    
    static const QStringList SCOPES;
    
    static const QRegExp DURATION_REGEXP;
    
    friend class YouTubeComment;
    friend class YouTubePlaylist;
    friend class YouTubeUser;
    friend class YouTubeVideo;  
};

#endif // YOUTUBE_H
