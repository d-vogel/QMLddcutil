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
    DDCA_Display_Info_List * dlist = ddca_get_displays();
    qDebug()<<"ddca_get_displays() returned "<< dlist;
    qDebug()<<dlist->ct << "display(s) were detected";
    for(int iDisp=0;iDisp<dlist->ct;iDisp++)
    {

        qDebug()<<"\nCreate a Display Identifier for display"<<iDisp <<
                  " : "<< dlist->info[iDisp].model_name;

        m_displayInfoList.append(dlist->info[iDisp]);

        rc = ddca_create_dispno_display_identifier(iDisp+1, &did); // ddcutil uses 1 paded indexing for displays

        char * did_repr = ddca_repr_display_identifier(did);

        qDebug()<<"did="<<did_repr;

        qDebug()<<"Create a display reference from the display identifier...";
        rc = ddca_create_display_ref(did, &dref);

        if (rc != 0) {
            qDebug()<<"ddct_get_display_ref() returned "<<
                      rc<< " ("<<ddca_status_code_name(rc) <<
                      "): "<< ddca_status_code_desc(rc);
        }
        else {
            char * dref_repr = ddca_repr_display_ref(dref);
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



                Version_Feature_Info* featureInfo;
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

    //    qDebug()<<"starting display";
    //    //display all result

    //    for(int iDisp=0;iDisp<m_displayHandleList.count();iDisp++)
    //    {
    //        QMap<QString, int>::iterator iVcp;
    //        for (iVcp = m_descrToVcp_perDisp.at(iDisp)->begin();
    //             iVcp!=m_descrToVcp_perDisp.at(iDisp)->end();++iVcp)
    //        {
    //            //            qDebug()<<iVcp.key();

    //            qDebug()<<iVcp.value()<<" : "<<iVcp.key();
    //            //            if(iVcp.value() != NULL)
    //            //            {
    //            //                qDebug()<<iVcp.key()<<" : "<<iVcp.value()->feature_name<<"-"<<iVcp.value()->desc;

    //            //                if((iVcp.value()->feature_flags & DDCA_SIMPLE_NC) == DDCA_SIMPLE_NC)
    //            //                {
    //            //                    for(int iVcpVal=0; iVcp.value()->sl_values[iVcpVal].value_code!=0;iVcpVal++)
    //            //                    {
    //            //                        qDebug()<<"\t"<< iVcp.value()->sl_values[iVcpVal].value_code
    //            //                               <<":"<< iVcp.value()->sl_values[iVcpVal].value_name;
    //            //                    }
    //            //                }
    //            //            }
    //            //            else
    //            //            {
    //            //                qDebug()<<"no Version_Feature_Info for "<<iVcp.key();
    //            //            }


    //        }
    //    }
    qDebug()<<"done with display";
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

long DDCutilController::brightness() const
{

    //FIXME: gets value for display 1
    Single_Vcp_Value *returnValue;
    ddca_get_vcp_value(m_displayHandleList.at(0),
                       m_descrToVcp_perDisp.at(0)->value("Brightness"),
                       NON_TABLE_VCP_VALUE, &returnValue);

    return (long)returnValue->val.c.cur_val;
}

long DDCutilController::brightnessMax() const
{
    Single_Vcp_Value *returnValue;
    ddca_get_vcp_value(m_displayHandleList.at(0),
                       m_descrToVcp_perDisp.at(0)->value("Brightness"),
                       NON_TABLE_VCP_VALUE, &returnValue);

    return (long) returnValue->val.c.max_val;

}

QList<QString> DDCutilController::whitePointList() const
{
    return m_vcpTovcpValueWithDescr_perDisp.at(0)
            ->value( m_descrToVcp_perDisp.at(0)->value("Select color preset") )
            ->values();
}

void DDCutilController::setBrightness(long value)
{
    int rc;
    qDebug()<<"trying to set new brightness:" << (int)value;
    //    qDebug()<<"nb of handles: "<<m_displayHandleList.count();
    for(int iDisp=0;iDisp<m_displayHandleList.count();iDisp++)
    {
        //        qDebug()<<"for disp: "<<iDisp;

        rc = ddca_set_continuous_vcp_value(m_displayHandleList.at(iDisp), m_descrToVcp_perDisp.at(iDisp)->value("Brightness"), (int)value);
        qDebug()<<"set brightness returned: "<<rc;
    }
}

QString DDCutilController::nameForDisplay(const unsigned int iDisplay)
{
    return QString(m_displayInfoList.at(iDisplay).model_name);
}

void DDCutilController::setWhitepoint(const QString &newWhitepoint)
{
    int rc;
    qDebug()<<"trying to set new whitepoint:" << newWhitepoint;
    for(int iDisp=0;iDisp<m_displayHandleList.count();iDisp++)
    {
        //        qDebug()<<"for disp: "<<iDisp;

        rc = ddca_set_continuous_vcp_value(m_displayHandleList.at(iDisp),
                                           m_descrToVcp_perDisp.at(iDisp)->value("Select color preset"),
                                           m_vcpTovcpValueWithDescr_perDisp.at(iDisp)->value(m_descrToVcp_perDisp.at(0)->value("Select color preset"))->key(newWhitepoint));
        qDebug()<<"set whitepoint returned: "<<rc;
    }
}

