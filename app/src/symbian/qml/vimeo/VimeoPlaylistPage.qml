/*
 * Copyright (C) 2016 Stuart Howarth <showarth@marxoft.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 3, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */

import QtQuick 1.1
import com.nokia.symbian 1.1
import cuteTube 2.0
import QVimeo 1.0 as QVimeo
import ".."

MyPage {
    id: root

    function load(playlistOrId) {
        playlist.loadPlaylist(playlistOrId);

        if (playlist.userId) {
            user.loadUser(playlist.userId);
            videosTab.model.list("/users/" + playlist.userId + "/albums/" + playlist.id + "/videos", {per_page: 50});
        }
    }

    title: view.currentTab.title
    showProgressIndicator: (playlist.status == QVimeo.ResourcesRequest.Loading)
                           || (user.status == QVimeo.ResourcesRequest.Loading)
                           || (videosTab.model.status == QVimeo.ResourcesRequest.Loading)
    tools: view.currentTab.tools ? view.currentTab.tools : ownTools
    onToolsChanged: if (status == PageStatus.Active) appWindow.pageStack.toolBar.setTools(tools, "set");

    ToolBarLayout {
        id: ownTools

        visible: false

        BackToolButton {}
    }

    VimeoPlaylist {
        id: playlist

        onStatusChanged: {
            switch (status) {
            case QVimeo.ResourcesRequest.Ready:
                if (!user.id) {
                    user.loadUser(userId);
                    videosTab.model.list("/users/" + userId + "/albums/" + id + "/videos", {per_page: 50});
                }

                break;
            case QVimeo.ResourcesRequest.Failed:
                infoBanner.showMessage(errorString);
                break;
            default:
                break;
            }
        }
    }

    VimeoUser {
        id: user

        onStatusChanged: if (status == QVimeo.ResourcesRequest.Failed) infoBanner.showMessage(errorString);
    }

    TabView {
        id: view

        anchors.fill: parent
        visible: playlist.id != ""

        Tab {
            id: infoTab

            width: view.width
            height: view.height
            title: qsTr("Album info")

            PlaylistThumbnail {
                id: thumbnail

                z: 10
                width: parent.width - platformStyle.paddingLarge * 2
                height: Math.floor(width * 3 / 4)
                anchors {
                    left: parent.left
                    top: parent.top
                    margins: platformStyle.paddingLarge
                }
                source: playlist.largeThumbnailUrl
                text: playlist.videoCount ? playlist.videoCount + " " + qsTr("videos") : qsTr("No videos")
                enabled: (videosTab.model.count > 0) && (Settings.videoPlayer == "cutetube")
                onClicked: {
                    var videos = [];

                    for (var i = 0; i < videosTab.model.count; i ++) {
                        videos.push(videosTab.model.get(i));
                    }

                    appWindow.pageStack.push(Qt.resolvedUrl("../VideoPlaybackPage.qml")).addVideos(videos);
                }
            }

            MyFlickable {
                id: flicker

                anchors {
                    left: parent.left
                    right: parent.right
                    top: thumbnail.bottom
                    topMargin: platformStyle.paddingLarge
                    bottom: parent.bottom
                }
                contentHeight: flow.height + platformStyle.paddingLarge * 2
                clip: true

                Flow {
                    id: flow

                    anchors {
                        left: parent.left
                        leftMargin: platformStyle.paddingLarge
                        right: parent.right
                        rightMargin: platformStyle.paddingLarge
                        top: parent.top
                    }
                    spacing: platformStyle.paddingLarge

                    Label {
                        width: parent.width
                        font.bold: true
                        wrapMode: Text.WordWrap
                        text: playlist.title
                    }

                    Avatar {
                        id: avatar

                        width: platformStyle.graphicSizeMedium
                        height: platformStyle.graphicSizeMedium
                        source: user.thumbnailUrl
                        onClicked: appWindow.pageStack.push(Qt.resolvedUrl("VimeoUserPage.qml")).load(user)
                    }

                    Item {
                        width: parent.width - avatar.width - platformStyle.paddingLarge
                        height: avatar.height

                        Label {
                            anchors {
                                left: parent.left
                                right: parent.right
                                top: parent.top
                            }
                            verticalAlignment: Text.AlignTop
                            elide: Text.ElideRight
                            font.pixelSize: platformStyle.fontSizeSmall
                            text: user.username
                        }

                        Label {
                            anchors {
                                left: parent.left
                                right: parent.right
                                bottom: parent.bottom
                            }
                            verticalAlignment: Text.AlignBottom
                            elide: Text.ElideRight
                            font.pixelSize: platformStyle.fontSizeSmall
                            font.weight: Font.Light
                            color: platformStyle.colorNormalMid
                            text: qsTr("Published on") + " " + playlist.date
                        }
                    }

                    Label {
                        width: parent.width
                        wrapMode: Text.Wrap
                        text: playlist.description ? Utils.toRichText(playlist.description) : qsTr("No description")
                        onLinkActivated: {
                            var resource = Resources.getResourceFromUrl(link);

                            if (resource.service != Resources.VIMEO) {
                                Qt.openUrlExternally(link);
                                return;
                            }

                            if (resource.type == Resources.USER) {
                                appWindow.pageStack.push(Qt.resolvedUrl("VimeoUserPage.qml")).load(resource.id);
                            }
                            else if (resource.type == Resources.PLAYLIST) {
                                appWindow.pageStack.push(Qt.resolvedUrl("VimeoPlaylistPage.qml")).load(resource.id);
                            }
                            else {
                                appWindow.pageStack.push(Qt.resolvedUrl("VimeoVideoPage.qml")).load(resource.id);
                            }
                        }
                    }
                }
            }

            MyScrollBar {
                flickableItem: flicker
            }

            states: State {
                name: "landscape"
                when: !appWindow.inPortrait

                PropertyChanges {
                    target: thumbnail
                    width: 280
                }

                AnchorChanges {
                    target: flicker
                    anchors {
                        left: thumbnail.right
                        top: parent.top
                    }
                }
            }
        }

        VimeoVideosTab {
            id: videosTab

            width: view.width
            height: view.height
            title: qsTr("Videos")
        }
    }

    Connections {
        target: videosTab.model
        onStatusChanged: {
            if ((videosTab.model.status == QVimeo.ResourcesRequest.Ready) && (videosTab.model.canFetchMore)) {
                videosTab.model.fetchMore();
            }
        }
    }
}
