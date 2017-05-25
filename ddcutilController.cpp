#include "ddcutilController.h"

DDCutilController::DDCutilController()
{
    QList<DDCA_Display_Handle> m_displayHandleList = QList<DDCA_Display_Handle>();
    QList<DDCA_Display_Info> m_displayInfoList = QList<DDCA_Display_Info>();

    //QList<DDCA_Capabilities> m_parsedCapabilities_perDisp = QList<DDCA_Capabilities>();

    QList<QMap<QString, int>*> m_descrToVcp_perDisp = QList<QMap<QString, int>*>();
    QList<QMap<int, QMap<int, QString>*>*> m_vcpTovcpValueWithDescr_perDisp =
            QList<QMap<int, QMap<int, QString>*>*>();
}

void DDCutilController::detect(){

    DDCA_Status rc;
    DDCA_Display_Identifier did;
    DDCA_Display_Ref dref;
    DDCA_Display_Handle dh = NULL;  // initialize to avoid clang analyzer warning



    qDebug()<<"\nCheck for monitors using ddca_get_displays()...\n";
    // Inquire about detected monitors.
    DDCA_Display_Info_List * dlist = ddca_get_display_info_list();
    qDebug()<<"ddca_get_displays() returned "<< dlist;
    qDebug()<<dlist->ct << "display(s) were detected";
    for(int iDisp=0;iDisp<dlist->ct;iDisp++)
    {

        qDebug()<<"\nCreate a Display Identifier for display"<<iDisp <<
                  " : "<< dlist->info[iDisp].model_name;

        m_displayInfoList.append(dlist->info[iDisp]);

        rc = ddca_create_dispno_display_identifier(iDisp+1, &did); // ddcutil uses 1 paded indexing for displays

        char * did_repr = ddca_did_repr(did);

        qDebug()<<"did="<<did_repr;

        qDebug()<<"Create a display reference from the display identifier...";
        rc = ddca_get_display_ref(did, &dref);

        if (rc != 0) {
            qDebug()<<"ddct_get_display_ref() returned "<<
                      rc<< " ("<<ddca_rc_name(rc) <<
                      "): "<< ddca_rc_desc(rc);
        }
        else {
            char * dref_repr = ddca_did_repr(dref);
            qDebug()<<"dref= " << dref_repr;

            qDebug()<<"Open the display reference, creating a display handle...";
            rc = ddca_open_display(dref, &dh);
            if (rc != 0) {
                qDebug()<<"ddct_open_display"<< rc;
            }
            else {
                qDebug()<<"adding handle to list";
                m_displayHandleList.append(dh);
                qDebug()<<"handles nb: "<<m_displayHandleList.count();
                char* capabilityString;
                DDCA_Capabilities* parsedCapabilities;
                ddca_get_capabilities_string(dh, &capabilityString);
                ddca_parse_capabilities_string(capabilityString, &parsedCapabilities);
                qDebug()<<parsedCapabilities->vcp_code_ct << "capabilities have been parsed";


                m_descrToVcp_perDisp.append(new QMap<QString, int>);
                m_vcpTovcpValueWithDescr_perDisp.append(new QMap<int, QMap<int, QString>*>);
                //fill the feature description to vcp LUT



                DDCA_Version_Feature_Info* featureInfo;
                for(int iVcp=0;iVcp<parsedCapabilities->vcp_code_ct;iVcp++)
                {

                    int vcpCode=parsedCapabilities->vcp_codes[iVcp].feature_code;

                    m_vcpTovcpValueWithDescr_perDisp.at(iDisp)->insert(vcpCode, new QMap<int, QString>);

                    m_descrToVcp_perDisp.at(iDisp)->insert(
                                QString(ddca_get_feature_name(vcpCode)), vcpCode);


                    ddca_get_feature_info_by_display(m_displayHandleList.at(iDisp), vcpCode, &featureInfo);
                    if(featureInfo != NULL)
                    {
                        qDebug()<<featureInfo->feature_code<<":"<<featureInfo->desc;
                        if((featureInfo->feature_flags & DDCA_SIMPLE_NC) == DDCA_SIMPLE_NC)
                        {
                            for(int iVcpVal=0;featureInfo->sl_values[iVcpVal].value_code!=0;++iVcpVal)
                            {


                                qDebug()<<"\t"<<featureInfo->sl_values[iVcpVal].value_code
                                       <<":"<< featureInfo->sl_values[iVcpVal].value_name;

                                bool thisVcpValIsSupported=false;

                                for(int iSupportedVcpVal=0; iSupportedVcpVal<parsedCapabilities->vcp_codes[iVcp].value_ct; iSupportedVcpVal++)
                                {
                                    if(parsedCapabilities->vcp_codes[iVcp].values[iSupportedVcpVal]
                                            ==featureInfo->sl_values[iVcpVal].value_code)
                                    {
                                        thisVcpValIsSupported=true;
                                    }
                                }

                                if(thisVcpValIsSupported)
                                {
                                    m_vcpTovcpValueWithDescr_perDisp.at(iDisp)->value(vcpCode)->insert(
                                                featureInfo->sl_values[iVcpVal].value_code,
                                                featureInfo->sl_values[iVcpVal].value_name);
                                    qDebug()<<"ADDED !";
                                }
                            }
                        }
                    }


                }
                qDebug()<<"done";
                //is this needed ? are variables created in the method automatically free'd ?
                ddca_free_parsed_capabilities(parsedCapabilities);
            }
        }
    }
    qDebug()<<"done with display";
}

unsigned int DDCutilController::monitorNumber() const
{
    return m_displayHandleList.count();
}



//bool DDCutilController::isSupported() const
//{
//    //    if (!m_resources) {
//    //        return false;
//    //    }

//    //    auto *outputs = xcb_randr_get_screen_resources_current_outputs(m_resources.data());
//    //    for (int i = 0; i < m_resources->num_outputs; ++i) {
//    //        if (backlight_get(outputs[i]) != -1) {
//    //            return true;
//    //        }
//    //    }

//    //    return false;
//}

long DDCutilController::brightness(const unsigned int dispIdx) const
{

    //FIXME: gets value for display 1
    DDCA_Single_Vcp_Value *returnValue;

        ddca_get_vcp_value(m_displayHandleList.at(dispIdx),
                           m_descrToVcp_perDisp.at(dispIdx)->value("Brightness"),
                           DDCA_NON_TABLE_VCP_VALUE, &returnValue);

    return (long)returnValue->val.c.cur_val;
}

long DDCutilController::brightnessMax(const unsigned int dispIdx) const
{
    DDCA_Single_Vcp_Value *returnValue;

        ddca_get_vcp_value(m_displayHandleList.at(dispIdx),
                           m_descrToVcp_perDisp.at(dispIdx)->value("Brightness"),
                           DDCA_NON_TABLE_VCP_VALUE, &returnValue);

    return (long) returnValue->val.c.max_val;

}

QList<QString> DDCutilController::whitePointList(const unsigned int dispIdx) const
{
    return m_vcpTovcpValueWithDescr_perDisp.at(dispIdx)
            ->value( m_descrToVcp_perDisp.at(dispIdx)->value("Select color preset") )
            ->values();
}

void DDCutilController::setBrightness(const unsigned int dispIdx, long value)
{
    int rc;
        rc = ddca_set_continuous_vcp_value(m_displayHandleList.at(dispIdx), m_descrToVcp_perDisp.at(dispIdx)->value("Brightness"), (int)value);
        qDebug()<<"set brightness returned: "<<rc;
}

QString DDCutilController::nameForDisplay(const unsigned int dispIdx)
{
    return QString(m_displayInfoList.at(dispIdx).model_name);
}

void DDCutilController::setWhitepoint(const unsigned int dispIdx, const QString &newWhitepoint)
{
    int rc;
    qDebug()<<"trying to set new whitepoint:" << newWhitepoint;

        rc = ddca_set_continuous_vcp_value(m_displayHandleList.at(dispIdx),
                                           m_descrToVcp_perDisp.at(dispIdx)->value("Select color preset"),
                                           m_vcpTovcpValueWithDescr_perDisp.at(dispIdx)->value(m_descrToVcp_perDisp.at(dispIdx)->value("Select color preset"))->key(newWhitepoint));
        qDebug()<<"set whitepoint returned: "<<rc;
}

