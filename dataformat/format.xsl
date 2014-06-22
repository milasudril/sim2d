<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:template match="/">
		<html>
			<head>
				<title><xsl:value-of select="fileformat/name" /> defintion</title>
				<link rel="stylesheet" type="text/css" href="format.css" />
			</head>
			<body>
				<h1><xsl:value-of select="fileformat/name" /></h1>
				<p>The status of this document is <xsl:value-of select="fileformat/status" /></p>
				<section>
					<h2>Description</h2>
					<xsl:copy-of select="fileformat/description/*" />
				</section>
				<section>
					<h2>Data encoding</h2>
					<p>
						A <dfn><xsl:copy-of select="fileformat/name" /></dfn> is
						encoded in
						<xsl:for-each select="fileformat/data_encoding/bits/field">
							<xsl:sort select="name"/>
							<xsl:value-of select="name"/>
							<xsl:if test="position()!=last()">
								<xsl:text>, </xsl:text>
							</xsl:if>
						</xsl:for-each>. 
					</p>
					<xsl:if test="count(fileformat/data_encoding/compressions)!=0">
						<p>The data stream can be compressed using any of the 
						following algorithms</p>
						<ul>
							<xsl:for-each select="fileformat/data_encoding/compressions/algorithm">
								<li><xsl:value-of select="name"/></li>
							</xsl:for-each>
						</ul>
					</xsl:if>
					<xsl:copy-of select="fileformat/data_encoding/comments/*" />
				</section>
				<section>
					<xsl:choose>
						<xsl:when 
							test="count(fileformat/data_encoding/bits/field[name='binary'])!=0">	
							<h2>File chunks</h2>
							<p>A <dfn><xsl:value-of select="fileformat/name" /></dfn>
							contains the following chunks:</p>
							<xsl:choose>
								<xsl:when test="fileformat/chunks/ordered='yes'">
									<ol>
										<xsl:for-each
											select="fileformat/chunks/chunk">
											<li><xsl:value-of select="name"/></li>
										</xsl:for-each>
									</ol>
								</xsl:when>
								<xsl:otherwise>
									<ul>
										<xsl:for-each
											select="fileformat/chunks/chunk">
											<li><xsl:value-of select="name"/></li>
										</xsl:for-each>
									</ul>
								</xsl:otherwise>
							</xsl:choose>
							<xsl:copy-of select="fileformat/chunks/comments/*" />
							<xsl:for-each select="fileformat/chunks/chunk">
								<section>
									<h3><xsl:value-of select="name"/></h3>
									<xsl:copy-of select="description/*"/>
									<xsl:if test="count(fields/field)!=0">
										<p>The <dfn><xsl:value-of select="name"/> </dfn>
										chunk contains the following fields:</p>
										<table>
											<xsl:for-each select="fields/field">
												<tr>
													<td><code class="type"><xsl:value-of select="type"/></code></td>
													<td><code><xsl:value-of select="name" /></code></td>
													<td><xsl:copy-of select="content" /></td>
												</tr>
											</xsl:for-each>
										</table>
									</xsl:if>
								</section>
							</xsl:for-each>
						</xsl:when>
						<xsl:otherwise>
							<h2>This template only works for binary file formats</h2>
						</xsl:otherwise>
					</xsl:choose>
				</section>
			</body>
		</html>
	</xsl:template>
</xsl:stylesheet> 
