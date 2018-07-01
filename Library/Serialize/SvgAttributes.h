///
/// @file Library/Serialize/SvgAttributes.h
///
/// SVG serialization implementation attributes
///

#pragma once
#ifndef __SVG_ATTRIBUTES_HEADER__
#define __SVG_ATTRIBUTES_HEADER__

#include <Serialize/Svg.h>

// SVG_ATTRIBUTE_CALLBACK
/// Set an element attribute value
/// @param Parser    The SVG parser
/// @param Attribute The SVG element attribute
/// @return Whether the attribute value was set or not
typedef
BOOLEAN
(EFIAPI
*SVG_ATTRIBUTE_CALLBACK) (
  IN SVG_PARSER    *Parser,
  IN XML_ATTRIBUTE *Attribute
);

// SVG_ATTRIBUTE
/// An SVG attribute callback
typedef struct SVG_ATTRIBUTE SVG_ATTRIBUTE;
struct SVG_ATTRIBUTE {

  // Name
  /// The SVG element name
  CONST CHAR16           *Name;
  // Callback
  /// The SVG element callback
  SVG_ATTRIBUTE_CALLBACK  Callback;

};
// SVG_ATTRIBUTES
/// SVG attributes
typedef struct SVG_ATTRIBUTES SVG_ATTRIBUTES;
struct SVG_ATTRIBUTES {

  // Count
  /// The SVG attribute count
  UINTN                Count;
  // Attributes
  /// The SVG attributes
  CONST SVG_ATTRIBUTE *Attributes;

};

// mSvgConditionalAttributes
/// The SVG conditional attributes
STATIC CONST SVG_ATTRIBUTE mSvgConditionalAttributes[] = {
  { L"requiredFeatures",   NULL }, // SvgRequiredFeaturesAttributeCallback
  { L"requiredExtensions", NULL }, // SvgRequiredExtensionsAttributeCallback
  { L"systemLanguage",     NULL }, // SvgSystemLanguageAttributeCallback
};
// mSvgCoreAttributes
/// The SVG core attributes
STATIC CONST SVG_ATTRIBUTE mSvgCoreAttributes[] = {
  { L"id",        NULL }, // SvgIdAttributeCallback
  { L"xml:base",  NULL }, // SvgXmlBaseAttributeCallback
  { L"xml:lang",  NULL }, // SvgXmlLangAttributeCallback
  { L"xml:space", NULL }, // SvgXmlSpaceAttributeCallback
};
// mSvgDocumentEventAttributes
/// The SVG document event attributes
STATIC CONST SVG_ATTRIBUTE mSvgDocumentEventAttributes[] = {
  { L"onunload", NULL }, // SvgOnUnloadAttributeCallback
  { L"onabort",  NULL }, // SvgOnAbortAttributeCallback
  { L"onerror",  NULL }, // SvgOnErrorAttributeCallback
  { L"onresize", NULL }, // SvgOnResizeAttributeCallback
  { L"onscroll", NULL }, // SvgOnScrollAttributeCallback
  { L"onzoom",   NULL }, // SvgOnZoomAttributeCallback
};
// mSvgGraphicEventAttributes
/// The SVG graphic event attributes
STATIC CONST SVG_ATTRIBUTE mSvgGraphicEventAttributes[] = {
  { L"onfocusin",   NULL }, // SvgOnFocusInAttributeCallback
  { L"onfocusout",  NULL }, // SvgOnFocusOutAttributeCallback
  { L"onactivate",  NULL }, // SvgOnActivateAttributeCallback
  { L"onclick",     NULL }, // SvgOnClickAttributeCallback
  { L"onmousedown", NULL }, // SvgOnMouseDownAttributeCallback
  { L"onmouseup",   NULL }, // SvgOnMouseUpAttributeCallback
  { L"onmouseover", NULL }, // SvgOnMouseOverAttributeCallback
  { L"onmousemove", NULL }, // SvgOnMouseMoveAttributeCallback
  { L"onmouseout",  NULL }, // SvgOnMouseOutAttributeCallback
  { L"onload",      NULL }, // SvgOnLoadAttributeCallback
};
// mSvgPresentationAttributes
/// The SVG presentation attributes
STATIC CONST SVG_ATTRIBUTE mSvgPresentationAttributes[] = {
  { L"alignment-baseline",           NULL }, // SvgAlignmentBaselineAttributeCallback
  { L"baseline-shift",               NULL }, // SvgBaselineShiftAttributeCallback
  { L"clip",                         NULL }, // SvgClipAttributeCallback
  { L"clip-path",                    NULL }, // SvgClipPathAttributeCallback
  { L"clip-rule",                    NULL }, // SvgClipRuleAttributeCallback
  { L"color",                        NULL }, // SvgColorAttributeCallback
  { L"color-interpolation",          NULL }, // SvgColorInterpolationAttributeCallback
  { L"color-interpolation-filters",  NULL }, // SvgColorInterpolationFiltersAttributeCallback
  { L"color-profile",                NULL }, // SvgColorProfileAttributeCallback
  { L"color-rendering",              NULL }, // SvgColorRenderingAttributeCallback
  { L"cursor",                       NULL }, // SvgCursorAttributeCallback
  { L"direction",                    NULL }, // SvgDirectionAttributeCallback
  { L"display",                      NULL }, // SvgDisplayAttributeCallback
  { L"dominant-baseline",            NULL }, // SvgDominantBaselineAttributeCallback
  { L"enable-background",            NULL }, // SvgEnableBackgroundAttributeCallback
  { L"fill",                         NULL }, // SvgFillAttributeCallback
  { L"fill-opacity",                 NULL }, // SvgFillOpacityAttributeCallback
  { L"fill-rule",                    NULL }, // SvgFillRuleAttributeCallback
  { L"filter",                       NULL }, // SvgFilterAttributeCallback
  { L"flood-color",                  NULL }, // SvgFloodColorAttributeCallback
  { L"flood-opacity",                NULL }, // SvgFloodCapacityAttributeCallback
  { L"font-family",                  NULL }, // SvgFontFamilyAttributeCallback
  { L"font-size",                    NULL }, // SvgFontSizeAttributeCallback
  { L"font-size-adjust",             NULL }, // SvgFontSizeAdjustAttributeCallback
  { L"font-stretch",                 NULL }, // SvgFontStretchAttributeCallback
  { L"font-style",                   NULL }, // SvgFontStyleAttributeCallback
  { L"font-variant",                 NULL }, // SvgFontVariantAttributeCallback
  { L"font-weight",                  NULL }, // SvgFontWeightAttributeCallback
  { L"glyph-orientation-horizontal", NULL }, // SvgGlyphOrientationHorizontalAttributeCallback
  { L"glyph-orientation-vertical",   NULL }, // SvgGlyphOrientationVerticalAttributeCallback
  { L"image-rendering",              NULL }, // SvgImageRenderingAttributeCallback
  { L"kerning",                      NULL }, // SvgKerningAttributeCallback
  { L"letter-spacing",               NULL }, // SvgLetterSpacingAttributeCallback
  { L"lighting-color",               NULL }, // SvgLightingColorAttributeCallback
  { L"marker-end",                   NULL }, // SvgMarkerEndAttributeCallback
  { L"marker-mid",                   NULL }, // SvgMarkerMidAttributeCallback
  { L"marker-start",                 NULL }, // SvgMarkerStartAttributeCallback
  { L"mask",                         NULL }, // SvgMaskAttributeCallback
  { L"opacity",                      NULL }, // SvgOpacityAttributeCallback
  { L"overflow",                     NULL }, // SvgOverflowAttributeCallback
  { L"pointer-events",               NULL }, // SvgPointerEventsAttributeCallback
  { L"shape-rendering",              NULL }, // SvgShapeRenderingAttributeCallback
  { L"stop-color",                   NULL }, // SvgStopColorAttributeCallback
  { L"stop-opacity",                 NULL }, // SvgStopOpacityAttributeCallback
  { L"stroke",                       NULL }, // SvgStrokeAttributeCallback
  { L"stroke-dasharray",             NULL }, // SvgStrokeDashArrayAttributeCallback
  { L"stroke-dashoffset",            NULL }, // SvgStrokeDashOffsetAttributeCallback
  { L"stroke-linecap",               NULL }, // SvgStrokeLineCapAttributeCallback
  { L"stroke-linejoin",              NULL }, // SvgStrokeLineJoinAttributeCallback
  { L"stroke-miterlimit",            NULL }, // SvgStrokeMiterLimitAttributeCallback
  { L"stroke-opacity",               NULL }, // SvgStrokeOpacityAttributeCallback
  { L"stroke-width",                 NULL }, // SvgStrokeWidthAttributeCallback
  { L"text-anchor",                  NULL }, // SvgTextAnchorAttributeCallback
  { L"text-decoration",              NULL }, // SvgTextDecorationAttributeCallback
  { L"text-rendering",               NULL }, // SvgTextRenderingAttributeCallback
  { L"unicode-bidi",                 NULL }, // SvgUnicodeBidiAttributeCallback
  { L"visibility",                   NULL }, // SvgVisibilityAttributeCallback
  { L"word-spacing",                 NULL }, // SvgWordSpaceAttributeCallback
  { L"writing-mode",                 NULL }, // SvgWritingModeAttributeCallback
};
// mSvgXlinkAttributes
/// The SVG XLINK attributes
STATIC CONST SVG_ATTRIBUTE mSvgXlinkAttributes[] = {
  { L"xlink:actuate", NULL }, // SvgXlinkTypeActuateAttributeCallback
  { L"xlink:arcrole", NULL }, // SvgXlinkArcroleAttributeCallback
  { L"xlink:href",    NULL }, // SvgXlinkHrefAttributeCallback
  { L"xlink:role",    NULL }, // SvgXlinkRoleAttributeCallback
  { L"xlink:show",    NULL }, // SvgXlinkShowAttributeCallback
  { L"xlink:title",   NULL }, // SvgXlinkTitleAttributeCallback
  { L"xlink:type",    NULL }, // SvgXlinkTypeAttributeCallback
};
// mSvgAttributes
/// The SVG element attributes
STATIC CONST SVG_ATTRIBUTE mSvgAttributes[] = {
  { L"baseProfile",               NULL }, // SvgBaseProfileAttributeCallback
  { L"class",                     NULL }, // SvgClassAttributeCallback
  { L"contentScriptType",         NULL }, // SvgContentScriptTypeAttributeCallback
  { L"contentStyleType",          NULL }, // SvgContentStyleTypeAttributeCallback
  { L"externalResourcesRequired", NULL }, // SvgExternalResourceRequiredAttributeCallback
  { L"height",                    NULL }, // SvgHeightAttributeCallback
  { L"preserveAspectRatio",       NULL }, // SvgPreserveAspectRatioAttributeCallback
  { L"style",                     NULL }, // SvgStyleAttributeCallback
  { L"viewBox",                   NULL }, // SvgViewBoxAttributeCallback
  { L"version",                   NULL }, // SvgVersionAttributeCallback
  { L"width",                     NULL }, // SvgWidthAttributeCallback
  { L"x",                         NULL }, // SvgXAttributeCallback
  { L"y",                         NULL }, // SvgYAttributeCallback
  { L"zoomAndPan",                NULL }, // SvgZoomAndPanAttributeCallback
};
// mSvgAAttributes
/// The SVG A element attributes
STATIC CONST SVG_ATTRIBUTE mSvgAAttributes[] = {
  { L"class",                     NULL }, // SvgClassAttributeCallback
  { L"externalResourcesRequired", NULL }, // SvgExternalResourcesRequiredAttributeCallback
  { L"style",                     NULL }, // SvgStyleAttributeCallback
  { L"target",                    NULL }, // SvgTargetAttributeCallback
  { L"transform",                 NULL }, // SvgTransformAttribtueCallback
  
};
// mSvgGlyphRefAttributes
/// The SVG GlyphRef element attributes
STATIC CONST SVG_ATTRIBUTE mSvgGlyphRefAttributes[] = {
  { L"class",    NULL }, // SvgClassAttributeCallback
  { L"dx",       NULL }, // SvgDxAttributeCallback
  { L"dy",       NULL }, // SvgDyAttributeCallback
  { L"format",   NULL }, // SvgFormatAttributeCallback
  { L"glyphRef", NULL }, // SvgGlyphRefAttributeCallback
  { L"style",    NULL }, // SvgStyleAttributeCallback
  { L"x",        NULL }, // SvgXAttributeCallback
  { L"y",        NULL }, // SvgYAttribtueCallback
};
// mSvgClipPathAttributes
/// The SVG ClipPath element attributes
STATIC CONST SVG_ATTRIBUTE mSvgClipPathAttributes[] = {
  { L"class",                     NULL }, // SvgClassAttributeCallback
  { L"clipPathUnits",             NULL }, // SvgClipPathUnitsAttributeCallback
  { L"externalResourcesRequired", NULL }, // SvgExternalResourcesRequiredAttributeCallback
  { L"transform",                 NULL }, // SvgTransformAttributeCallback
  { L"style",                     NULL }, // SvgStyleAttributeCallback
};
// mSvgColorProfileAttributes
/// The SVG color-profile element attributes
STATIC CONST SVG_ATTRIBUTE mSvgColorProfileAttributes[] = {
  { L"local",             NULL }, // SvgLocalAttributeCallback },
  { L"name",              NULL }, // SvgNameAttributeCallback },
  { L"rendering-intent" , NULL }, // SvgRenderingIntentAttributeCallback },
};
// mSvgCursorAttributes
/// The SVG cursor element attributes
STATIC CONST SVG_ATTRIBUTE mSvgCursorAttributes[] = {
  { L"externalResourcesRequired", NULL }, // SvgExternalResourcesRequiredAttributeCallback },
  { L"x",                         NULL }, // SvgXAttributeCallback },
  { L"y",                         NULL }, // SvgYAttributeCallback },
};
// mSvgFilterAttributes
/// The SVG filter element attributes
STATIC CONST SVG_ATTRIBUTE mSvgFilterAttributes[] = {
  { L"class",                     NULL }, // SvgClassElementCallback },
  { L"externalResourcesRequired", NULL }, // SvgeExternalResourcesRequiredElementCallback },
  { L"filterRes",                 NULL }, // SvgFilterResElementCallback },
  { L"filterUnits",               NULL }, // SvgFilterUnitsElementCallback },
  { L"height",                    NULL }, // SvgHeightElementCallback },
  { L"primitiveUnits",            NULL }, // SvgPrimitiveUnitsElementCallback },
  { L"style",                     NULL }, // SvgStyleElementCallback },
  { L"width",                     NULL }, // SvgWidthElementCallback },
  { L"x",                         NULL }, // SvgXElementCallback },
  { L"y",                         NULL }, // SvgYElementCallback },
};
// mSvgFontAttributes
/// The SVG font element attributes
STATIC CONST SVG_ATTRIBUTE mSvgFontAttributes[] = {
  { L"class",                     NULL }, // SvgClassAttributeCallback },
  { L"externalResourcesRequired", NULL }, // SvgExternalResourcesRequiredAttributeCallback },
  { L"horiz-adv-x",               NULL }, // SvgHorizAdvXAttributeCallback },
  { L"horiz-origin-x",            NULL }, // SvgHorizOriginXAttributeCallback },
  { L"horiz-origin-y",            NULL }, // SvgHorizOriginYAttributeCallback },
  { L"style",                     NULL }, // SvgStyleAttributeCallback },
  { L"vert-adv-y",                NULL }, // SvgVertAdvYAttributeCallback },
  { L"vert-origin-x",             NULL }, // SvgVertOriginXAttributeCallback },
  { L"vert-origin-y",             NULL }, // SvgVertOriginYAttributeCallback },
};
// mSvgFontFaceAttributes
/// The SVG font-face element attributes
STATIC CONST SVG_ATTRIBUTE mSvgFontFaceAttributes[] = {
  { L"alphabetic",              NULL }, // SvgAlphabeticAttributeCallback },
  { L"ascent",                  NULL }, // SvgAscentAttributeCallback },
  { L"accent-height",           NULL }, // SvgAccentHeightAttributeCallback },
  { L"bbox",                    NULL }, // SvgBBoxAttributeCallback },
  { L"cap-height",              NULL }, // SvgCapHeightAttributeCallback },
  { L"descent",                 NULL }, // SvgDescentAttributeCallback },
  { L"font-family",             NULL }, // SvgFontFamilyAttributeCallback },
  { L"font-size",               NULL }, // SvgFontSizeAttributeCallback },
  { L"font-stretch",            NULL }, // SvgFontStretchAttributeCallback },
  { L"font-style",              NULL }, // SvgFontStyleAttributeCallback },
  { L"font-weight",             NULL }, // SvgFontWeightAttributeCallback },
  { L"font-variant",            NULL }, // SvgFontVariantAttributeCallback },
  { L"hanging",                 NULL }, // SvgHangingAttributeCallback },
  { L"ideographic",             NULL }, // SvgIdeographicAttributeCallback },
  { L"mathematical",            NULL }, // SvgMathematicalAttributeCallback },
  { L"panose-1",                NULL }, // SvgPanose1AttributeCallback },
  { L"overline-position",       NULL }, // SvgOverlinePositionAttributeCallback },
  { L"overline-thickness",      NULL }, // SvgOverlineThicknessAttributeCallback },
  { L"slope",                   NULL }, // SvgSlopeAttributeCallback },
  { L"stemh",                   NULL }, // SvgStemHAttributeCallback },
  { L"stemv",                   NULL }, // SvgStemVAttributeCallback },
  { L"strikethrough-position",  NULL }, // SvgStrikethroughPositionAttributeCallback },
  { L"strikethrough-thickness", NULL }, // SvgStrikethroughThicknessAttributeCallback },
  { L"underline-position",      NULL }, // SvgUnderlinePositionAttributeCallback },
  { L"underline-thickness",     NULL }, // SvgUnderlineThicknessAttributeCallback }
  { L"unicode-range",           NULL }, // SvgUnicodeRangeAttributeCallback },
  { L"units-per-em",            NULL }, // SvgUnitsPeremAttributeCallback },
  { L"v-alphabetic",            NULL }, // SvgVAlphabeticAttributeCallback },
  { L"v-hanging",               NULL }, // SvgVHangingAttributeCallback },
  { L"v-ideographic",           NULL }, // SvgVIdeographicAttributeCallback },
  { L"v-mathematical",          NULL }, // SvgVMathematicalAttributeCallback },
  { L"widths",                  NULL }, // SvgWidthsAttributeCallback },
};
// mSvgForeignObjectAttributes
/// The SVG foreign-object element attributes
STATIC CONST SVG_ATTRIBUTE mSvgForeignObjectAttributes[] = {
  { L"class",                     NULL }, // SvgClassAttributeCallback },  
  { L"externalResourcesRequired", NULL }, // SvgExternalResourcesRequiredAttributeCallback },
  { L"height",                    NULL }, // SvgHeightAttributeCallback },
  { L"style",                     NULL }, // SvgStyleAttributeCallback },
  { L"transform",                 NULL }, // SvgTransformAttributeCallback },
  { L"width",                     NULL }, // SvgWidthAttributeCallback },
  { L"x",                         NULL }, // SvgXAttributeCallback },
  { L"y",                         NULL }, // SvgYAttributeCallback },
};
// mSvgImageAttributes
/// The SVG image element attributes
STATIC CONST SVG_ATTRIBUTE mSvgImageAttributes[] = {
  { L"class",                     NULL }, // SvgClassAttributeCallback },  
  { L"externalResourcesRequired", NULL }, // SvgExternalResourcesRequiredAttributeCallback },
  { L"height",                    NULL }, // SvgHeightAttributeCallback },
  { L"preserveAspectRatio",       NULL }, // SvgPreserveAspectRatio },
  { L"style",                     NULL }, // SvgStyleAttributeCallback },
  { L"transform",                 NULL }, // SvgTransformAttributeCallback },
  { L"width",                     NULL }, // SvgWidthAttributeCallback },
  { L"x",                         NULL }, // SvgXAttributeCallback },
  { L"y",                         NULL }, // SvgYAttributeCallback },
};
// mSvgMarkerAttributes
/// The SVG marker element attributes
STATIC CONST SVG_ATTRIBUTE mSvgMarkerAttributes[] = {
  { L"class",                     NULL }, // SvgClassAttributeCallback },
  { L"externalResourcesRequired", NULL }, // SvgExternalResourcesRequiredAttributeCallback },
  { L"markerHeight",              NULL }, // SvgMarkerHeightAttributeCallback },
  { L"markerUnits",               NULL }, // SvgMarkerUnitsAttributeCallback },
  { L"markerWidth",               NULL }, // SvgMarkerWidthAttributeCallback },
  { L"orient",                    NULL }, // SvgOrientAttributeCallback },
  { L"preserveAspectRatio",       NULL }, // SvgPreserveAspectRatioAttributeCallback },
  { L"refX",                      NULL }, // SvgRefXAttributeCallback },
  { L"refY",                      NULL }, // SvgRefYAttributeCallback },
  { L"style",                     NULL }, // SvgStyleAttributeCallback },
  { L"viewBox",                   NULL }, // SvgViewBoxAttributeCallback },
};
// mSvgMaskAttributes
/// The SVG mask element attributes
STATIC CONST SVG_ATTRIBUTE mSvgMaskAttributes[] = {
  { L"class",                     NULL }, // SvgClassAttributeCallback },
  { L"externalResourcesRequired", NULL }, // SvgExternalResourcesRequiredAttributeCallback },
  { L"height",                    NULL }, // SvgHeightAttributeCallback },
  { L"maskContentUnits",          NULL }, // SvgMaskContentUnitsAttributeCallback },
  { L"maskUnits",                 NULL }, // SvgMaskUnitsAttributeCallback },
  { L"style",                     NULL }, // SvgStyleAttributeCallback },
  { L"width",                     NULL }, // SvgWidthAttributeCallback },
  { L"x",                         NULL }, // SvgXAttributeCallback },
  { L"y",                         NULL }, // SvgYAttributeCallback },
};
// mSvgPatternAttributes
/// The SVG pattern element attributes
STATIC CONST SVG_ATTRIBUTE mSvgPatternAttributes[] = {
  { L"class",                     NULL }, // SvgClassAttributeCallback },
  { L"externalResourcesRequired", NULL }, // SvgExternalResourcesRequiredAttributeCallback },
  { L"height",                    NULL }, // SvgHeightAttributeCallback },
  { L"patternContentUnits",       NULL }, // SvgPatternContentUnitsAttributeCallback },
  { L"patternTransform",          NULL }, // SvgPatternTransformAttributeCallback },
  { L"patternUnits",              NULL }, // SvgPatternUnitsAttributeCallback },
  { L"preserveAspectRatio",       NULL }, // SvgPreserveAspectRatioAttributeCallback },
  { L"style",                     NULL }, // SvgStyleAttributeCallback },
  { L"viewBox",                   NULL }, // SvgViewBoxAttributeCallback },
  { L"width",                     NULL }, // SvgWidthAttributeCallback },
  { L"x",                         NULL }, // SvgXAttributeCallback },
  { L"y",                         NULL }, // SvgYAttributeCallback },
};
// mSvgScriptAttributes
/// The SVG script element attributes
STATIC CONST SVG_ATTRIBUTE mSvgScriptAttributes[] = {
  { L"externalResourcesRequired", NULL }, // SvgExternalResourcesRequiredAttributeCallback },
  { L"type",                      NULL }, // SvgTypeAttributeCallback },
};
// mSvgStyleAttributes
/// The SVG style element attributes
STATIC CONST SVG_ATTRIBUTE mSvgStyleAttributes[] = {
  { L"media", NULL }, // SvgMediaAttributeCallback },
  { L"title", NULL }, // SvgTitleAttributeCallback },
  { L"type",  NULL }, // SvgTypeAttributeCallback },
};
// mSvgSwitchAttributes
/// The SVG switch element attributes
STATIC CONST SVG_ATTRIBUTE mSvgSwitchAttributes[] = {
  { L"class",                     NULL }, // SvgClassAttributeCallback },
  { L"externalResourcesRequired", NULL }, // SvgExternalResourcesRequiredAttributeCallback },
  { L"style",                     NULL }, // SvgStyleAttributeCallback },
  { L"transform",                 NULL }, // SvgTransformAttributeCallback },
};
// mSvgTextAttributes
/// The SVG text element attributes
STATIC CONST SVG_ATTRIBUTE mSvgTextAttributes[] = {
  { L"class",                     NULL }, // SvgClassAttributeCallback },
  { L"dx",                        NULL }, // SvgDxAttributeCallback },
  { L"dy",                        NULL }, // SvgDyAttributeCallback },
  { L"externalResourcesRequired", NULL }, // SvgExternalResourcesRequiredAttributeCallback },
  { L"lengthAdjust",              NULL }, // SvgLengthAdjustAttributeCallback },
  { L"rotate",                    NULL }, // SvgRotateAttributeCallback },
  { L"style",                     NULL }, // SvgStyleAttributeCallback },
  { L"textLength",                NULL }, // SvgTextLengthAttributeCallback },
  { L"transform",                 NULL }, // SvgTransformAttributeCallback },  
  { L"x",                         NULL }, // SvgXAttributeCallback },
  { L"y",                         NULL }, // SvgYAttributeCallback },
};
// mSvgViewAttributes
/// The SVG view element attributes
STATIC CONST SVG_ATTRIBUTE mSvgViewAttributes[] = {
  { L"externalResourcesRequired", NULL }, // SvgExternalResourcesRequiredAttributeCallback },
  { L"preserveAspectRatio",       NULL }, // SvgPreserveAspectRatioAttributeCallback },
  { L"viewBox",                   NULL }, // SvgViewBoxAttributeCallback },
  { L"viewTarget",                NULL }, // SvgViewTargetAttributeCallback },
  { L"zoomAndPan",                NULL }, // SvgZoomAndPanAttributeCallback },
};

#endif // __SVG_ATTRIBUTES_HEADER__
