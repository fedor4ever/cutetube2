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

#ifndef PLUGINCATEGORIESPAGE_H
#define PLUGINCATEGORIESPAGE_H

#include "page.h"
#include "plugincategorymodel.h"

class QListView;
class QVBoxLayout;

class PluginCategoriesPage : public Page
{
    Q_OBJECT

public:
    explicit PluginCategoriesPage(const QString &service, QWidget *parent = 0);

    Status status() const;

public Q_SLOTS:
    virtual void cancel();
    virtual void reload();
    
    void list(const QString &resourceId);
    void search(const QString &query, const QString &order);

private Q_SLOTS:
    void showVideos(const QModelIndex &index);

    void onModelStatusChanged(ResourcesRequest::Status status);

private:
    void setStatus(Status s);
    
    PluginCategoryModel *m_model;

    QListView *m_view;

    QVBoxLayout *m_layout;

    Status m_status;
};

#endif // PLUGINCATEGORIESPAGE_H
