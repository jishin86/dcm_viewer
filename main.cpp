#include <iostream>


#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcdeftag.h"

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmdata/dcistrmf.h"

#include "dcmtk/dcmimgle/dcmimage.h"


int main(int argc, char** argv) {

    for(int i=0; i<argc; i++ ){ std::cout << "arg" << i << "=" << argv[i] << std::endl; }

    DcmFileFormat *file = new DcmFileFormat();



    if(argc>1) {



        //char filename ="test.dcm"
        OFString filename(argv[1]);

        OFCondition condition;

        condition = file->loadFile(filename);

        std::cout << condition.good() << std::endl;

        if (condition.bad()) {
            std::cerr << "wrong file=" << filename << std::endl;
            return 0;
        }


        DcmDataset *data = file->getDataset();


        //print all tag
        //if(!data->isEmpty()) {data->print(COUT);} else { return 0; }


        //iteration


        DcmObject *obj = nullptr;
        DcmElement *elem = nullptr;
        DcmStack stack;
        DcmTagKey key;
        OFCondition cond = data->nextObject(stack, OFTrue);
        const DcmDataDictionary & dict = dcmDataDict.rdlock();
        dcmDataDict.unlock();
        DcmDictEntry *tmpEntry = nullptr;

        DcmTag  tmpTag;
        OFCondition cond2;

        OFString tmpS; //std::string
        Float32 tmpFL; //float
        Float64 tmpFD; //double
        long int longint;

        int nobj=0;

        while(cond.good())
        {
            std::cout << nobj++;
            obj = stack.top();
            std::cout << " Leaf : " << obj->isLeaf();
            tmpTag=obj->getTag();
            std::cout << " Tag : " << tmpTag.toString().c_str(); // decimal

            tmpEntry = const_cast<DcmDictEntry*>(dict.findEntry(obj->getTag(), NULL));
            if(tmpEntry == NULL) {

                std::cout << " ERROR: unknown Tag";
                tmpTag= DCM_UndefinedTagKey;
                //break;

            } else std::cout << " TagName : " << tmpEntry->getTagName();


            //if(!obj->isLeaf()) { // not print pixels
            cond2 = data->findAndGetElement(tmpTag, elem, OFFalse);

            if (cond2.good()) {
                elem->getOFString(tmpS, 0);
                std::cout << " # Attribute : " << tmpS;
            };
            //}

            std::cout << " # GTag : " << obj->getGTag(); //hexa
            std::cout << " ETag : " << obj->getETag();  // hexa
            std::cout << " VR name : " << obj->getTag().getVRName();

            std::cout << " VR num : " << obj->getVR();
            std::cout << " Ident=" << obj->ident(); //DcmEVR

            std::cout << " VM : " << obj->getVM();

            std::cout << " length : " << obj->getLength();
            elem= static_cast<DcmElement*>(obj);
            //std::cout << " length : " << elem->getFloat64()


            std::cout << std::endl;
            cond = data->nextObject(stack, OFTrue);

        }


        //check tags related with CT image

        std::cout << "Check tags related with CT image." << std::endl;

        cond = data->findAndGetElement(DCM_PixelData,elem);
        std::cout<< cond.good() << " DCM_PixelData" << std::endl;

        const int length = elem->getLength();
        std::cout<< " pixeldata  length" << length<< std::endl;

        cond= data->findAndGetOFString(DCM_PhotometricInterpretation,tmpS);
        std::cout<< cond.good() << " DCM_PhotometricInterpretation=" << tmpS << std::endl;

        tmpS.clear();
        cond= data->findAndGetOFString(DCM_PixelRepresentation,tmpS,0);
        std::cout<< cond.good() <<  " DCM_PixelRepresentation=" << tmpS << std::endl;

        //Uint16 slope, intercept;
        float slope, intercept;
        cond=data->findAndGetOFString(DCM_RescaleSlope,tmpS);
        slope=atof(tmpS.c_str());
        std::cout << cond.good() << " slope=" << slope << " " << tmpS << std::endl;

        cond=data->findAndGetOFString(DCM_RescaleIntercept, tmpS);
        intercept=atof(tmpS.c_str());
        std::cout << cond.good() <<" intercept=" << intercept << " " << tmpS << std::endl;

        cond=data->findAndGetOFString(DCM_ImageType,tmpS,0);
        std::cout << cond.good() << " DCM_ImageType0=" << tmpS << std::endl;

        cond=data->findAndGetOFString(DCM_ImageType,tmpS,1);
        std::cout << cond.good() << " DCM_ImageType1=" << tmpS << std::endl;

        cond=data->findAndGetOFString(DCM_ImageType,tmpS,2);
        std::cout << cond.good() << " DCM_ImageType2=" << tmpS << std::endl;


        Float64 spacing[2];
        cond=data->findAndGetFloat64(DCM_PixelSpacing, spacing[0],0);
        cond=data->findAndGetFloat64(DCM_PixelSpacing, spacing[1],1);
        std::cout<< cond.good() << " DCM_PixelSpacing=" << spacing[0] << " mm" << spacing[1] << " mm" << std::endl;


        Float64 thickness;
        cond=data->findAndGetFloat64(DCM_SliceThickness, thickness,0);
        std::cout<< cond.good() << " DCM_SliceThickness=" << thickness<< " mm" << std::endl;


        Float64 zpos;
        cond=data->findAndGetFloat64(DCM_SliceLocation, zpos);
        std::cout<< cond.good() << " DCM_SliceLocation=" << zpos<< std::endl;

        Uint16 size[2];
        cond=data->findAndGetUint16(DCM_Rows, size[0], 0);
        data->findAndGetUint16(DCM_Columns, size[1], 0);
        std::cout<< cond.good() << " DCM_Rows=" << size[0] << " DCM_Columns=" << size[1] << std::endl;


        Float64 position[3];
        cond=data->findAndGetFloat64(DCM_ImagePositionPatient, position[0], 0);
        data->findAndGetFloat64(DCM_ImagePositionPatient, position[1], 1);
        data->findAndGetFloat64(DCM_ImagePositionPatient, position[2], 2);
        std::cout<< cond.good() << " DCM_ImagePositionPatient=( " << position[0] << ", " << position[1] << ", " << position[2] << " )" << std::endl;


        data->findAndGetOFString(DCM_BitsAllocated,tmpS);
        std::cout << cond.good() << " DCM_BitsAllocated=" << tmpS << " ";
        data->findAndGetOFString(DCM_BitsStored,tmpS);
        std::cout << cond.good() << " DCM_BitsStored=" << tmpS << " ";
        data->findAndGetOFString(DCM_HighBit,tmpS);
        std::cout << cond.good() << " DCM_HighBit=" << tmpS << std::endl;


    }


    return 0;
}