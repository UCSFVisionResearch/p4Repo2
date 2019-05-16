/**
 * @file FixationStability.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include <cstdio>
#include "comm/SerialComm.h"
#include "utils/Utils.h"
#include "utils/ModelUtils.h"
#include "FixationStability.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

namespace fstab {

	CEyeParameters::CEyeParameters(EYE eye, int dev, const etu::CColorSpec& color,
								   const CrossLocations_t& unoccluded,
								   const CrossLocations_t& occluded)
		: m_eye(eye),
		  m_deviation(dev),
		  m_color(color),
		  m_unoccluded(unoccluded.begin(), unoccluded.end()),
		  m_occluded(occluded.begin(), occluded.end())
	{}

	CEyeParameters::~CEyeParameters() { /* Nothing to do */ }

	EYE CEyeParameters::eye() const { return m_eye; }
	int CEyeParameters::deviation() const { return m_deviation; }
	const etu::CColorSpec& CEyeParameters::color() const { return m_color; }
	const CrossLocations_t& CEyeParameters::unoccluded() const { return m_unoccluded; }
	const CrossLocations_t& CEyeParameters::occluded() const { return m_occluded; }

	CEyeParameters* CEyeParameters::getFrom(FILE* infile)
	{
		int eye, dev;
		int size = 0;
		int temp;
		etu::CColorSpec* color = NULL;
		CrossLocations_t occluded, unoccluded;
		CEyeParameters* retval = NULL;

		if (fscanf(infile, "%d\n%d\n", &eye, &dev) == 2) {
			if ((color = etu::CColorSpec::getFrom(infile)) != NULL) {

				fscanf(infile, "%d\t", &size);
				for (int ix = 0; ix < size; ix++) {
					fscanf(infile, "%d\t", &temp);
					unoccluded.push_back(temp / 2.0);
				}

				fscanf(infile, "\n");

				fscanf(infile, "%d\t", &size);
				for (int ix = 0; ix < size; ix++) {
					fscanf(infile, "%d\t", &temp);
					occluded.push_back(temp / 2.0);
				}
				
				retval = new CEyeParameters((EYE)eye, dev, (*color), 
					unoccluded, occluded);

				delete color;
			}
		}

		return retval;
	}

	void CEyeParameters::putTo(FILE* outfile) 
	{
		fprintf(outfile, "%d\n%d\n", m_eye, m_deviation);

		m_color.putTo(outfile);
		
		fprintf(outfile, "%d\t", m_unoccluded.size());
		for (const_LocationIter_t curr = m_unoccluded.begin(); 
			 curr < m_unoccluded.end(); ++curr) 
		{
			fprintf(outfile, "%d\t", (int)((*curr) * 2));
		}
		fprintf(outfile, "\n");

		fprintf(outfile, "%d\t", m_occluded.size());
		for (const_LocationIter_t curr = m_occluded.begin(); 
			 curr < m_occluded.end(); ++curr) 
		{
			fprintf(outfile, "%d\t", (int)((*curr) * 2));
		}
		fprintf(outfile, "\n");
	}

} // namespace fstab
