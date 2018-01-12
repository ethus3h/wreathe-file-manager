// This file is generated by kconfig_compiler_kf5 from dolphin_directoryviewpropertysettings.kcfg.
// All changes you do to this file will be lost.
#ifndef VIEWPROPERTYSETTINGS_H
#define VIEWPROPERTYSETTINGS_H

#include <views/dolphinview.h>
#include <qnamespace.h>

#include <qglobal.h>
#include <kconfigskeleton.h>
#include <QCoreApplication>
#include <QDebug>

#include <kfileitemdelegate.h>
class ViewPropertySettings : public KConfigSkeleton
{
  public:

    ViewPropertySettings( KSharedConfig::Ptr config = KSharedConfig::openConfig() );
    ~ViewPropertySettings();

    /**
      Set Show hidden files
    */
    void setShowHiddenFiles( bool v )
    {
      if (!isImmutable( QStringLiteral( "ShowHiddenFiles" ) ))
        mShowHiddenFiles = v;
    }

    /**
      Get Show hidden files
    */
    bool showHiddenFiles() const
    {
      return mShowHiddenFiles;
    }

    /**
      Set Hide specific files
    */
    void setHideSpecificFiles( const QStringList & v )
    {
      if (!isImmutable( QStringLiteral( "HideSpecificFiles" ) ))
        mHideSpecificFiles = v;
    }

    /**
      Get Hide specific files
    */
    QStringList hideSpecificFiles() const
    {
      return mHideSpecificFiles;
    }

    /**
      Set Version
    */
    void setVersion( int v )
    {
      if (v < 1)
      {
        qDebug() << "setVersion: value " << v << " is less than the minimum value of 1";
        v = 1;
      }
      if (!isImmutable( QStringLiteral( "Version" ) ))
        mVersion = v;
    }

    /**
      Get Version
    */
    int version() const
    {
      return mVersion;
    }

    /**
      Set View Mode
    */
    void setViewMode( int v )
    {
      if (v < 2)
      {
        qDebug() << "setViewMode: value " << v << " is less than the minimum value of 2";
        v = 2;
      }

      if (v > DolphinView::MaxModeEnum)
      {
        qDebug() << "setViewMode: value " << v << " is greater than the maximum value of DolphinView::MaxModeEnum";
        v = DolphinView::MaxModeEnum;
      }

      if (!isImmutable( QStringLiteral( "ViewMode" ) ))
        mViewMode = v;
    }

    /**
      Get View Mode
    */
    int viewMode() const
    {
      return mViewMode;
    }

    /**
      Set Show preview
    */
    void setShowPreview( bool v )
    {
      if (!isImmutable( QStringLiteral( "ShowPreview" ) ))
        mShowPreview = v;
    }

    /**
      Get Show preview
    */
    bool showPreview() const
    {
      return mShowPreview;
    }

    /**
      Set Categorized Sorting
    */
    void setCategorizedSorting( bool v )
    {
      if (!isImmutable( QStringLiteral( "CategorizedSorting" ) ))
        mCategorizedSorting = v;
    }

    /**
      Get Categorized Sorting
    */
    bool categorizedSorting() const
    {
      return mCategorizedSorting;
    }

    /**
      Set Sort files by
    */
    void setSorting( int v )
    {
      if (v < 0)
      {
        qDebug() << "setSorting: value " << v << " is less than the minimum value of 0";
        v = 0;
      }

      if (v > DolphinView::MaxSortingEnum)
      {
        qDebug() << "setSorting: value " << v << " is greater than the maximum value of DolphinView::MaxSortingEnum";
        v = DolphinView::MaxSortingEnum;
      }

      if (!isImmutable( QStringLiteral( "Sorting" ) ))
        mSorting = v;
    }

    /**
      Get Sort files by
    */
    int sorting() const
    {
      return mSorting;
    }

    /**
      Set Order in which to sort files
    */
    void setSortOrder( int v )
    {
      if (v < Qt::AscendingOrder)
      {
        qDebug() << "setSortOrder: value " << v << " is less than the minimum value of Qt::AscendingOrder";
        v = Qt::AscendingOrder;
      }

      if (v > Qt::DescendingOrder)
      {
        qDebug() << "setSortOrder: value " << v << " is greater than the maximum value of Qt::DescendingOrder";
        v = Qt::DescendingOrder;
      }

      if (!isImmutable( QStringLiteral( "SortOrder" ) ))
        mSortOrder = v;
    }

    /**
      Get Order in which to sort files
    */
    int sortOrder() const
    {
      return mSortOrder;
    }

    /**
      Set Show folders first when sorting files and folders
    */
    void setSortFoldersFirst( bool v )
    {
      if (!isImmutable( QStringLiteral( "SortFoldersFirst" ) ))
        mSortFoldersFirst = v;
    }

    /**
      Get Show folders first when sorting files and folders
    */
    bool sortFoldersFirst() const
    {
      return mSortFoldersFirst;
    }

    /**
      Set Additional information (deprecated, use AdditionInfoV2 instead)
    */
    void setAdditionalInfo( int v )
    {
      if (!isImmutable( QStringLiteral( "AdditionalInfo" ) ))
        mAdditionalInfo = v;
    }

    /**
      Get Additional information (deprecated, use AdditionInfoV2 instead)
    */
    int additionalInfo() const
    {
      return mAdditionalInfo;
    }

    /**
      Set Additional information
    */
    void setAdditionalInfoV2( const QStringList & v )
    {
      if (!isImmutable( QStringLiteral( "AdditionalInfoV2" ) ))
        mAdditionalInfoV2 = v;
    }

    /**
      Get Additional information
    */
    QStringList additionalInfoV2() const
    {
      return mAdditionalInfoV2;
    }

    /**
      Set Properties last changed
    */
    void setTimestamp( const QDateTime & v )
    {
      if (!isImmutable( QStringLiteral( "Timestamp" ) ))
        mTimestamp = v;
    }

    /**
      Get Properties last changed
    */
    QDateTime timestamp() const
    {
      return mTimestamp;
    }

  protected:

    // Settings
    bool mShowHiddenFiles;
    QStringList mHideSpecificFiles;

    // Dolphin
    int mVersion;
    int mViewMode;
    bool mShowPreview;
    bool mCategorizedSorting;
    int mSorting;
    int mSortOrder;
    bool mSortFoldersFirst;
    int mAdditionalInfo;
    QStringList mAdditionalInfoV2;
    QDateTime mTimestamp;

  private:
};

#endif

