///
/// @file Library/Serialize/SvgElements.h
///
/// SVG serialization implementation elements
///

#pragma once
#ifndef __SVG_ELEMENTS_HEADER__
#define __SVG_ELEMENTS_HEADER__

#include "SvgAttributes.h"

// SVG_ELEMENT_CALLBACK
/// Modify the SVG image using an SVG element
/// @param Parser  The SVG parser
/// @param Element The SVG element
/// @return Whether the SVG image element is valid or not
typedef
BOOLEAN
(EFIAPI
*SVG_ELEMENT_CALLBACK) (
  IN SVG_PARSER *Parser,
  IN XML_TREE   *Element
);

// SVG_ELEMENT
/// An SVG element callback
typedef struct SVG_ELEMENT SVG_ELEMENT;
struct SVG_ELEMENT {

  // Name
  /// The SVG element name
  CONST CHAR16         *Name;
  // Callback
  /// The SVG element callback
  SVG_ELEMENT_CALLBACK  Callback;

};
// SVG_ELEMENTS
/// SVG elements
typedef struct SVG_ELEMENTS SVG_ELEMENTS;
struct SVG_ELEMENTS {

  // Count
  /// The SVG element count
  UINTN              Count;
  // Elements
  /// The SVG elements
  CONST SVG_ELEMENT *Elements;

};

// SvgElementCallback
/// Modify the SVG image using an SVG element
/// @param Parser  The SVG parser
/// @param Element The SVG element name
/// @return Whether the SVG image element is valid or not
STATIC
BOOLEAN
EFIAPI
SvgElementCallback (
  IN SVG_PARSER *Parser,
  IN XML_TREE   *Element
);

// mSvgStructuralElements
/// The SVG structural elements
STATIC CONST SVG_ELEMENT mSvgStructuralElements[] = {
  { L"defs",   NULL }, // SvgDefsElementCallback },
  { L"g",      NULL }, // SvgGElementCallback },
  { L"svg",    SvgElementCallback },
  { L"symbol", NULL }, // SvgSymbolElementCallback },
  { L"use",    NULL }, // SvgUseElementCallback },
};
// mSvgAnimationElements
/// The SVG animation elements
STATIC CONST SVG_ELEMENT mSvgAnimationElements[] = {
  { L"animate",          NULL }, // SvgAnimateElementCallback },
  { L"animateColor",     NULL }, // SvgAnimateColorElementCallback },
  { L"animateMotion",    NULL }, // SvgAnimateMotionElementCallback },
  { L"animateTransform", NULL }, // SvgAnimateTransformElementCallback },
  { L"set",              NULL }, // SvgSetElementCallback },
};
// mSvgDescriptiveElements
/// The SVG descriptive elements
STATIC CONST SVG_ELEMENT mSvgDescriptiveElements[] = {
  { L"desc",     NULL }, // SvgDescElementCallback },
  { L"metadata", NULL }, // SvgMetadataElementCallback },
  { L"title",    NULL }, // SvgTitleElementCallback },
};
// mSvgGradientElements
/// The SVG gradient elements
STATIC CONST SVG_ELEMENT mSvgGradientElements[] = {
  { L"linearGradient", NULL }, // SvgLinearGradientElementCallback },
  { L"radialGradient", NULL }, // SvgRadialGradientElementCallback },
};
// mSvgShapeElements
/// The SVG shape elements
STATIC CONST SVG_ELEMENT mSvgShapeElements[] = {
  { L"circle",   NULL }, // SvgCircleElementCallback },
  { L"ellipse",  NULL }, // SvgEllipseElementCallback },
  { L"line",     NULL }, // SvgLineElementCallback },
  { L"path",     NULL }, // SvgPathElementCallback },
  { L"polygon",  NULL }, // SvgPolygonElementCallback },
  { L"polyline", NULL }, // SvgPolylineElementCallback },
  { L"rect",     NULL }, // SvgRectElementCallback },
};
// mSvgTextElements
/// The SVG text elements
STATIC CONST SVG_ELEMENT mSvgTextElements[] = {
  { L"altGlyph", NULL }, // SvgAltGlyphElementCallback },
  { L"textPath", NULL }, // SvgTextPathElementCallback },
  { L"tref",     NULL }, // SvgTrefElementCallback },
  { L"tspan",    NULL }, // SvgTspanElementCallback },
};
// mSvgFilterPrimitiveElements
/// The SVG filter primitive elements
STATIC CONST SVG_ELEMENT mSvgFilterPrimitiveElements[] = {
  { L"feBlend",             NULL }, // SvgfeBlendElementCallback },
  { L"feColorMatrix",       NULL }, // SvgfeColorMatrixElementCallback },
  { L"feComponentTransfer", NULL }, // SvgfeComponentTransferElementCallback },
  { L"feComposite",         NULL }, // SvgfeCompositeElementCallback },
  { L"feConvolveMatrix",    NULL }, // SvgfeConvolveMatrixElementCallback },
  { L"feDiffuseLighting",   NULL }, // SvgfeDiffuseLightingElementCallback },
  { L"feDisplacementMap",   NULL }, // SvgfeDisplacementMapElementCallback },
  { L"feFlood",             NULL }, // SvgfeFloodElementCallback },
  { L"feGaussianBlur",      NULL }, // SvgfeGaussianBlurElementCallback },
  { L"feImage",             NULL }, // SvgfeImageElementCallback },
  { L"feMerge",             NULL }, // SvgfeMergeElementCallback },
  { L"feMorphology",        NULL }, // SvgfeMorphologyElementCallback },
  { L"feOffset",            NULL }, // SvgfeOffsetElementCallback },
  { L"feSpecularLighting",  NULL }, // SvgfeSpecularLightingElementCallback },
  { L"feTile",              NULL }, // SvgfeTileElementCallback },
  { L"feTurbulence",        NULL }, // SvgfeTurbulenceElementCallback },
};
// mSvgElements
/// The SVG container elements
STATIC CONST SVG_ELEMENT mSvgElements[] = {
  { L"altGlyphDef",   NULL }, // SvgAltGlyphDefElementCallback },
  { L"clipPath",      NULL }, // SvgClipPathElementCallback },
  { L"color-profile", NULL }, // SvgColorProfileElementCallback },
  { L"cursor",        NULL }, // SvgCursorElementCallback },
  { L"filter",        NULL }, // SvgFilterElementCallback },
  { L"font",          NULL }, // SvgFontElementCallback },
  { L"font-face",     NULL }, // SvgFontFaceElementCallback },
  { L"foreignObject", NULL }, // SvgForeignObjectElementCallback },
  { L"image",         NULL }, // SvgImageElementCallback },
  { L"marker",        NULL }, // SvgMarkerElementCallback },
  { L"mask",          NULL }, // SvgMaskElementCallback },
  { L"pattern",       NULL }, // SvgPatternElementCallback },
  { L"script",        NULL }, // SvgScriptElementCallback },
  { L"style",         NULL }, // SvgStyleElementCallback },
  { L"switch",        NULL }, // SvgSwitchElementCallback },
  { L"text",          NULL }, // SvgTextElementCallback },
  { L"view",          NULL }, // SvgViewElementCallback },
};
// mSvgAltGlyphElements
/// The SVG alternate glyph elements
STATIC CONST SVG_ELEMENT mSvgAlgGlyphElements[] = {
  { L"altGlyphItem", NULL }, // SvgAltGlyphItemElementCallback },
  { L"glyphRef",     NULL }, // SvgGlyphRefElementCallback },
};
// mSvgClipPathElements
/// The SVG ClipPath elements
STATIC CONST SVG_ELEMENT mSvgClipPathElements[] = {
  { L"text", NULL }, // SvgTextElementCallback },
  { L"use",  NULL }, // SvgUseElementCallback },
};
// mSvgFilterElements
/// The SVG filter elements
STATIC CONST SVG_ELEMENT mSvgFilterElements[] = {
  { L"animate", NULL }, // SvgAnimateElementCallback },
  { L"set",     NULL }, // SvgSetElementCallback },
};
// mSvgFontElements
/// The SVG font elements
STATIC CONST SVG_ELEMENT mSvgFontElements[] = {
  { L"font-face",     NULL }, // SvgFontFaceElementCallback },
  { L"glyph",         NULL }, // SvgGlyphElementCallback },
  { L"hkern",         NULL }, // SvgHKernElementCallback },
  { L"missing-glyph", NULL }, // SvgMissingGlyphElementCallback },
  { L"vkern",         NULL }, // SvgVKernElementCallback },
};
// mSvgSwitchElements
/// The SVG switch elements
STATIC CONST SVG_ELEMENT mSvgSwitchElements[] = {
  { L"a",             NULL }, // SvgaElementCallback },
  { L"foreignObject", NULL }, // SvgforeignObjectElementCallback },
  { L"g",             NULL }, // SvggElementCallback },
  { L"image",         NULL }, // SvgimageElementCallback },
  { L"svg",           NULL }, // SvgsvgElementCallback },
  { L"switch",        NULL }, // SvgswitchElementCallback },
  { L"text",          NULL }, // SvgtextElementCallback },
  { L"use",           NULL }, // SvguseElementCallback },
};

#endif // __SVG_ELEMENTS_HEADER__
