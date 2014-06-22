<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="param[1]">
	<span class="typename"><xsl:value-of select="type"/></span>
	<xsl:text> </xsl:text>
	<span class="varname"><xsl:value-of select="declname"/></span>
</xsl:template>

<xsl:template match="para">
<xsl:value-of select="." />
</xsl:template>

<xsl:template match="briefdescription">
<xsl:apply-templates select="para"/>
</xsl:template>

<xsl:template match="detaileddescription" mode="parafirst">
<xsl:apply-templates select="para[1]"/>
</xsl:template>

<xsl:template match="parameterlist">
<h4>Parameters</h4>
<table>
<xsl:for-each select="parameteritem">
<tr>
<td class="varname"><xsl:value-of select="parameternamelist/parametername"/></td>
<td><xsl:apply-templates select="parameterdescription/para" /></td>
</tr>
</xsl:for-each>
</table>
</xsl:template>

<xsl:template match="detaileddescription" mode="other">
<xsl:if test="count(para)>1">
	<xsl:for-each select="para[position()>1]">
		<xsl:if test="count(./parameterlist)=0">
			<p><xsl:apply-templates select="."/></p>
		</xsl:if>
	</xsl:for-each>
	<xsl:if test="count(para/parameterlist)>0">
		<xsl:apply-templates select="para/parameterlist" />
	</xsl:if>
</xsl:if>
</xsl:template>

<xsl:template match="type">
<span class="typename">
	<xsl:choose>
		<xsl:when test="count(ref)=0">
			<xsl:value-of select="."/>
		</xsl:when>
		<xsl:otherwise>
			<a>
			<xsl:attribute name="href">
				<xsl:value-of select="ref/@refid"/>.html
			</xsl:attribute>
			<xsl:value-of select="."/>
			</a>
		</xsl:otherwise>
	</xsl:choose>
</span>
</xsl:template>

<xsl:template match="param">
	<span class="paramrow">
		<xsl:text>,</xsl:text>
		<xsl:apply-templates select="type"/>
		<xsl:text> </xsl:text>
		<span class="varname"><xsl:value-of select="declname"/></span>
	</span>
</xsl:template>

<xsl:template match="memberdef" mode="membername">
<span class="varname"><xsl:value-of select="name" /></span>
	<xsl:if test="@kind='function'">
		(<xsl:apply-templates select="param"/>)
		<xsl:if test="@const='yes'">
		<span class="resword">const</span>
		</xsl:if>
	</xsl:if>
</xsl:template>

<xsl:template match="memberdef" mode="brief">
<tr>
	<td><xsl:apply-templates select="type" /></td>
	<td>
		<xsl:apply-templates select="." mode="membername" />
	</td>
	<td class="briefdescription"><xsl:apply-templates select="briefdescription" /></td>
</tr>
</xsl:template>

<xsl:template match="memberdef" mode="detail">
<h3>
<xsl:apply-templates select="type" /><xsl:text> </xsl:text>
<xsl:apply-templates select="." mode="membername" />
</h3>
<p>
<xsl:apply-templates select="briefdescription" />
<xsl:apply-templates select="detaileddescription" mode="parafirst"/>
</p>
<xsl:apply-templates select="detaileddescription" mode="other"/>
</xsl:template>



<xsl:template match="/">
<html>
<head>
	<title><xsl:value-of select="doxygen/compounddef/compoundname" /></title>
	<link rel="stylesheet" href="format.css" type="text/css" />
</head>
<body>
<form id="search" action="search.html" method="get">
<label>Search:</label><xsl:text> </xsl:text><input type="text" name="namestring" id="namestring" />
</form>
<h1>
	<span class="resword"><xsl:value-of select="doxygen/compounddef/@kind"/></span>
	<xsl:text> </xsl:text>
	<xsl:value-of select="doxygen/compounddef/compoundname" />
</h1>
<p><xsl:apply-templates select="doxygen/compounddef/briefdescription" />
<xsl:apply-templates select="doxygen/compounddef/detaileddescription" mode="parafirst"/>
</p>
<xsl:apply-templates select="doxygen/compounddef/detaileddescription" mode="other"/>
<p id="include">Defined in
	<span class="filename"><a>
		<xsl:attribute name="href">
			<xsl:value-of select="doxygen/compounddef/includes/@refid"/>.html
		</xsl:attribute>
		<xsl:value-of select="doxygen/compounddef/includes" />
	</a></span>
</p>
<h2>List of all Members</h2>
<p>This section lists all members of the <xsl:value-of select="doxygen/compounddef/@kind"/><xsl:text> </xsl:text> <span class="typename"><xsl:value-of select="doxygen/compounddef/compoundname" /></span>.</p>
<xsl:for-each select="doxygen/compounddef/sectiondef">
<h3><xsl:value-of select="@kind" /></h3>
<table>
<xsl:apply-templates select="memberdef" mode="brief">
	<xsl:sort select="name"/>
</xsl:apply-templates>
</table>
</xsl:for-each>
<h2>Member descriptions</h2>
<xsl:apply-templates select="doxygen/compounddef/sectiondef/memberdef" mode="detail">
	<xsl:sort select="name"/>
</xsl:apply-templates>
</body>
</html>
</xsl:template>
</xsl:stylesheet>