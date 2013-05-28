/* === This file is part of Tomahawk Player - <http://tomahawk-player.org> ===
 *
 *   Copyright 2013, Christian Muehlhaeuser <muesli@tomahawk-player.org>
 *   Copyright 2013, Teo Mrnjavac <teo@kde.org>
 *
 *   Tomahawk is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Tomahawk is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Tomahawk. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TRACK_H
#define TRACK_H

// #include "Typedefs.h"
// #include "TrackData.h"

#include "PlaybackLog.h"

#include "trackdata_ptr.h"
#include "query_wptr.h"
#include "track_wptr.h"
#include "track_ptr.h"
#include "source_ptr.h"
#include "track_ptr.h"
#include "query_ptr.h"
#include "artist_ptr.h"
#include "album_ptr.h"

//
// #include <QObject>
// #include <QList>
#include <QVariantMap>

#include "DllMacro.h"

class QPixmap;
class QSize;

namespace Tomahawk
{

class SocialAction;

class DLLEXPORT Track : public QObject
{
Q_OBJECT

friend class Pipeline;

public:
    enum DescriptionMode
    { Detailed = 0, Short = 1 };

    static track_ptr get( const QString& artist, const QString& track, const QString& album = QString(), int duration = 0, const QString& composer = QString(), unsigned int albumpos = 0, unsigned int discnumber = 0 );
    static track_ptr get( unsigned int id, const QString& artist, const QString& track, const QString& album, int duration, const QString& composer, unsigned int albumpos, unsigned int discnumber );

    virtual ~Track();

//    void setArtist( const QString& artist ) { m_artist = artist; updateSortNames(); }
    void setAlbum( const QString& album ) { m_album = album; updateSortNames(); }
//    void setTrack( const QString& track ) { m_track = track; updateSortNames(); }
/*    void setDuration( int duration ) { m_duration = duration; }
    void setAlbumPos( unsigned int albumpos ) { m_albumpos = albumpos; }
    void setDiscNumber( unsigned int discnumber ) { m_discnumber = discnumber; }
    void setComposer( const QString& composer ) { m_composer = composer; updateSortNames(); }*/

    bool equals( const Tomahawk::track_ptr& other, bool ignoreCase = false ) const;

    QVariant toVariant() const;
    QString toString() const;
    Tomahawk::query_ptr toQuery();

    QString composerSortname() const { return m_composerSortname; }
    QString albumSortname() const { return m_albumSortname; }
    QString artistSortname() const;
    QString trackSortname() const;

    QString artist() const;
    QString track() const;
    QString composer() const { return m_composer; }
    QString album() const { return m_album; }
    int duration() const { return m_duration; }
    int year() const;
    unsigned int albumpos() const { return m_albumpos; }
    unsigned int discnumber() const { return m_discnumber; }

    Tomahawk::artist_ptr artistPtr() const;
    Tomahawk::album_ptr albumPtr() const;
    Tomahawk::artist_ptr composerPtr() const;

#ifndef ENABLE_HEADLESS
    QPixmap cover( const QSize& size, bool forceLoad = true ) const;
#endif
    bool coverLoaded() const;

    void setLoved( bool loved, bool postToInfoSystem = true );
    bool loved();

    void share( const Tomahawk::source_ptr& source );

    void loadAttributes();
    QVariantMap attributes() const;

    void loadStats();
    QList< Tomahawk::PlaybackLog > playbackHistory( const Tomahawk::source_ptr& source = Tomahawk::source_ptr() ) const;
    unsigned int playbackCount( const Tomahawk::source_ptr& source = Tomahawk::source_ptr() );

    void loadSocialActions();
    QList< Tomahawk::SocialAction > allSocialActions() const;
    QString socialActionDescription( const QString& action, DescriptionMode mode ) const;

    QList<Tomahawk::query_ptr> similarTracks() const;
    QStringList lyrics() const;

    unsigned int trackId() const;

    QWeakPointer< Tomahawk::Track > weakRef() { return m_ownRef; }
    void setWeakRef( QWeakPointer< Tomahawk::Track > weakRef ) { m_ownRef = weakRef; }

    void startPlaying();
    void finishPlaying( int timeElapsed );

signals:
    void coverChanged();
    void socialActionsLoaded();
    void attributesLoaded();
    void statsLoaded();
    void similarTracksLoaded();
    void lyricsLoaded();

    void updated();

public slots:
    void deleteLater();

private:
    explicit Track( unsigned int id, const QString& artist, const QString& track, const QString& album, int duration, const QString& composer, unsigned int albumpos, unsigned int discnumber );
    explicit Track( const QString& artist, const QString& track, const QString& album, int duration, const QString& composer, unsigned int albumpos, unsigned int discnumber );

    void init();

    void updateSortNames();

    QString m_composer;
    QString m_album;
    QString m_composerSortname;
    QString m_albumSortname;

    int m_duration;
    unsigned int m_albumpos;
    unsigned int m_discnumber;

    mutable Tomahawk::artist_ptr m_artistPtr;
    mutable Tomahawk::album_ptr m_albumPtr;
    mutable Tomahawk::artist_ptr m_composerPtr;

    mutable trackdata_ptr m_trackData;

    query_wptr m_query;
    QWeakPointer< Tomahawk::Track > m_ownRef;

    static QHash< QString, track_wptr > s_tracksByName;
};

}; //ns

Q_DECLARE_METATYPE( Tomahawk::track_ptr );

#endif // TRACK_H
