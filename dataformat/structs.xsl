<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="text"/>
<xsl:template match="/">
/**<xsl:value-of select="fileformat/name" />
<xsl:copy-of select="fileformat/description/*" />
*/

<xsl:if test="count(fileformat/data_encoding/compressions)!=0">
/**Compression algorithms
*/
enum
	{
<xsl:for-each select="fileformat/data_encoding/compressions/algorithm">
	<xsl:value-of select="name"/>,
</xsl:for-each>
	} 
</xsl:if>

<xsl:choose>
	<xsl:when test="count(fileformat/data_encoding/bits/field[name='binary'])!=0">	
<xsl:for-each select="fileformat/chunks/chunk">

/**<xsl:copy-of select="description/*"/>
*/
struct <xsl:value-of select="name"/>
	{
	<xsl:for-each select="fields/field">
	/**<xsl:copy-of select="content" />
	*/
	<xsl:value-of select="type"/><xsl:text> </xsl:text><xsl:value-of select="name" />;
	</xsl:for-each>
	};

</xsl:for-each>
</xsl:when>
<xsl:otherwise>
#error "This template only works for binary file formats"
</xsl:otherwise>
</xsl:choose>
</xsl:template>
</xsl:stylesheet> 
