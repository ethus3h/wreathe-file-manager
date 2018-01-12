// This file is generated by kconfig_compiler_kf5 from dolphin_compactmodesettings.kcfg.
// All changes you do to this file will be lost.
#ifndef COMPACTMODESETTINGS_H
#define COMPACTMODESETTINGS_H

#include <kconfigskeleton.h>
#include <QCoreApplication>
#include <QDebug>

#include <QFontDatabase>
#include <kiconloader.h>
class CompactModeSettings : public KConfigSkeleton
{
  public:

    static CompactModeSettings *self();
    ~CompactModeSettings();

    /**
      Set Use system font
    */
    static
    void setUseSystemFont( bool v )
    {
      if (!self()->isImmutable( QStringLiteral( "UseSystemFont" ) ))
        self()->mUseSystemFont = v;
    }

    /**
      Get Use system font
    */
    static
    bool useSystemFont()
    {
      return self()->mUseSystemFont;
    }

    /**
      Set Font family
    */
    static
    void setFontFamily( const QString & v )
    {
      if (!self()->isImmutable( QStringLiteral( "FontFamily" ) ))
        self()->mFontFamily = v;
    }

    /**
      Get Font family
    */
    static
    QString fontFamily()
    {
      return self()->mFontFamily;
    }

    /**
      Set Font size
    */
    static
    void setFontSize( double v )
    {
      if (!self()->isImmutable( QStringLiteral( "FontSize" ) ))
        self()->mFontSize = v;
    }

    /**
      Get Font size
    */
    static
    double fontSize()
    {
      return self()->mFontSize;
    }

    /**
      Set Italic
    */
    static
    void setItalicFont( bool v )
    {
      if (!self()->isImmutable( QStringLiteral( "ItalicFont" ) ))
        self()->mItalicFont = v;
    }

    /**
      Get Italic
    */
    static
    bool italicFont()
    {
      return self()->mItalicFont;
    }

    /**
      Set Font weight
    */
    static
    void setFontWeight( int v )
    {
      if (!self()->isImmutable( QStringLiteral( "FontWeight" ) ))
        self()->mFontWeight = v;
    }

    /**
      Get Font weight
    */
    static
    int fontWeight()
    {
      return self()->mFontWeight;
    }

    /**
      Set Icon size
    */
    static
    void setIconSize( int v )
    {
      if (!self()->isImmutable( QStringLiteral( "IconSize" ) ))
        self()->mIconSize = v;
    }

    /**
      Get Icon size
    */
    static
    int iconSize()
    {
      return self()->mIconSize;
    }

    /**
      Set Preview size
    */
    static
    void setPreviewSize( int v )
    {
      if (!self()->isImmutable( QStringLiteral( "PreviewSize" ) ))
        self()->mPreviewSize = v;
    }

    /**
      Get Preview size
    */
    static
    int previewSize()
    {
      return self()->mPreviewSize;
    }

    /**
      Set Maximum text width index (0 means unlimited)
    */
    static
    void setMaximumTextWidthIndex( int v )
    {
      if (!self()->isImmutable( QStringLiteral( "MaximumTextWidthIndex" ) ))
        self()->mMaximumTextWidthIndex = v;
    }

    /**
      Get Maximum text width index (0 means unlimited)
    */
    static
    int maximumTextWidthIndex()
    {
      return self()->mMaximumTextWidthIndex;
    }

  protected:
    CompactModeSettings();
    friend class CompactModeSettingsHelper;


    // CompactMode
    bool mUseSystemFont;
    QString mFontFamily;
    double mFontSize;
    bool mItalicFont;
    int mFontWeight;
    int mIconSize;
    int mPreviewSize;
    int mMaximumTextWidthIndex;

  private:
};

#endif

