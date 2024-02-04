import React, { useState, useEffect } from "react";
import PropTypes from "prop-types";

import styled from "styled-components";

import { Fetch, Flex, RedButton, Button, buttonStyle, cPrimary, Alert,Confirmation, Spinner } from "./UiComponents";
import { FiFile as File, FiTrash2 as Trash2, FiDownload as Download } from "react-icons/fi";

import Config from "./../configuration.json";
let loc;
if (Config.find(entry => entry.name === "language")) {
    loc = require("./../lang/" + Config.find(entry => entry.name === "language").value + ".json");
} else {
    loc = require("./../lang/en.json");
}


export function FirmwareListing(props) {
    const [state, setState] = useState({ firmwareurl: ""});
    const [FwUpdate, setFwUpdate] = useState(false);

    useEffect(() => {
        fetchData();
    }, []);

    function fetchData() {
        fetch(`${props.API}/api/firmwareurl/get`)
            .then((response) => {
                return response.json();
            })
            .then((data) => {
                setState(data);
                console.log("webfirmwaredata: Data",data);
            });
            console.log("Firmware Url:",state.firmwareurl);
        }

    function DoFirmwareUpdate() {
        console.log("Do FirrmwareUpdate");
        fetch(`${props.API}/api/firmwareurl/DoUpdate`, { method: "POST" });
        }
    
    
    let header;
    header = loc.firmwareTitle;

    //</Flex><h3>{header}</h3>{list}</>;
    // <div><Upload action={`${props.API}/upload`} onFinished={fetchData} filter={props.filter} /></div>
    // return <><h3>{header}</h3></>;

    return <><h3>{header}</h3><Flex>
        {<div>{state.firmwareurl}</div>}
    </Flex>
    <Flex>
         <div><Button onClick={() => { setFwUpdate(true);}}>{loc.firwmwareUpdateBtn}</Button></div> 
     </Flex>

     <Confirmation active={FwUpdate}
    confirm={() =>{setFwUpdate(false);DoFirmwareUpdate();}}
    cancel={() => setFwUpdate(false)}>{loc.firwmwareUpdateQuest}</Confirmation>
     </>; 
    

}

FirmwareListing.propTypes = {
    API: PropTypes.string,
};

