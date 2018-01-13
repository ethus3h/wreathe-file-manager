// This file is generated by kconfig_compiler_kf5 from dolphin_detailsmodesettings.kcfg.
// All changes you do to this file will be lost.

#include "dolphin_detailsmodesettings.h"

DetailsModeSettings::DetailsModeSettings(  )
  : KConfigSkeleton( QStringLiteral( "dolphinrc" ) )
{
  setCurrentGroup( QStringLiteral( "DetailsMode" ) );

  KConfigSkeleton::ItemString  *itemFontFamily;
  itemFontFamily = new KConfigSkeleton::ItemString( currentGroup(), QStringLiteral( "FontFamily" ), mFontFamily, KGlobalSettings::generalFont().family() );
  addItem( itemFontFamily, QStringLiteral( "FontFamily" ) );
  KConfigSkeleton::ItemBool  *itemUseSystemFont;
  itemUseSystemFont = new KConfigSkeleton::ItemBool( currentGroup(), QStringLiteral( "UseSystemFont" ), mUseSystemFont, true );
  addItem( itemUseSystemFont, QStringLiteral( "UseSystemFont" ) );
  KConfigSkeleton::ItemDouble  *itemFontSize;
  itemFontSize = new KConfigSkeleton::ItemDouble( currentGroup(), QStringLiteral( "FontSize" ), mFontSize, KGlobalSettings::generalFont().pointSizeF() );
  addItem( itemFontSize, QStringLiteral( "FontSize" ) );
  KConfigSkeleton::ItemBool  *itemItalicFont;
  itemItalicFont = new KConfigSkeleton::ItemBool( currentGroup(), QStringLiteral( "ItalicFont" ), mItalicFont, false );
  addItem( itemItalicFont, QStringLiteral( "ItalicFont" ) );
  KConfigSkeleton::ItemInt  *itemFontWeight;
  itemFontWeight = new KConfigSkeleton::ItemInt( currentGroup(), QStringLiteral( "FontWeight" ), mFontWeight, 0 );
  addItem( itemFontWeight, QStringLiteral( "FontWeight" ) );
  KConfigSkeleton::ItemInt  *itemIconSize;
  itemIconSize = new KConfigSkeleton::ItemInt( currentGroup(), QStringLiteral( "IconSize" ), mIconSize, KIconLoader::SizeSmall );
  addItem( itemIconSize, QStringLiteral( "IconSize" ) );
  KConfigSkeleton::ItemInt  *itemPreviewSize;
  itemPreviewSize = new KConfigSkeleton::ItemInt( currentGroup(), QStringLiteral( "PreviewSize" ), mPreviewSize, KIconLoader::SizeLarge );
  addItem( itemPreviewSize, QStringLiteral( "PreviewSize" ) );
  KConfigSkeleton::ItemBool  *itemExpandableFolders;
  itemExpandableFolders = new KConfigSkeleton::ItemBool( currentGroup(), QStringLiteral( "ExpandableFolders" ), mExpandableFolders, false );
  addItem( itemExpandableFolders, QStringLiteral( "ExpandableFolders" ) );
  QList<int> defaultColumnPositions;
  defaultColumnPositions.append( 0 );
  defaultColumnPositions.append( 1 );
  defaultColumnPositions.append( 2 );
  defaultColumnPositions.append( 3 );
  defaultColumnPositions.append( 4 );
  defaultColumnPositions.append( 5 );
  defaultColumnPositions.append( 6 );
  defaultColumnPositions.append( 7 );
  defaultColumnPositions.append( 8 );

  KConfigSkeleton::ItemIntList  *itemColumnPositions;
  itemColumnPositions = new KConfigSkeleton::ItemIntList( currentGroup(), QStringLiteral( "ColumnPositions" ), mColumnPositions, defaultColumnPositions );
  addItem( itemColumnPositions, QStringLiteral( "ColumnPositions" ) );
}

DetailsModeSettings::~DetailsModeSettings()
{
}

