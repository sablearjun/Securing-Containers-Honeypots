import * as React from 'react';
import OutlinedInput from '@mui/material/OutlinedInput';
import InputLabel from '@mui/material/InputLabel';
import MenuItem from '@mui/material/MenuItem';
import FormControl from '@mui/material/FormControl';
import ListItemText from '@mui/material/ListItemText';
import Select from '@mui/material/Select';
import Checkbox from '@mui/material/Checkbox';

const ITEM_HEIGHT = 48;
const ITEM_PADDING_TOP = 8;
const MenuProps = {
  PaperProps: {
    style: {
      maxHeight: ITEM_HEIGHT * 4.5 + ITEM_PADDING_TOP,
      width: 250,
    },
  },
};


const NodeSelectDropdown = ({ data,nodeIdentifier, setNodeIdentifier, isDisabled }: { data: any , nodeIdentifier : any, setNodeIdentifier: any, isDisabled: boolean}) =>{
  // data.map((it:any, index:any) => console.log(it.hostname, index));
  // console.log("data: ", data);
  // console.log("item: ", data[0]);
  const [variantName, setVariantName] = React.useState([]);

  const handleChange = (event: any) => {
    const {
      target: { value },
    } = event;

    const filterdValue = value.filter(
      (item:any) => variantName.findIndex((o:any) => o.id === item.id) >= 0
    );

    let duplicatesRemoved = value.filter((item:any, itemIndex:any) =>
      value.findIndex((o:any, oIndex:any) => o.id === item.id && oIndex !== itemIndex)
    );

    let duplicateRemoved:any = [];

    value.forEach((item:any) => {
      if (duplicateRemoved.findIndex((o:any) => o.id === item.id) >= 0) {
        duplicateRemoved = duplicateRemoved.filter((x:any) => x.id === item.id);
      } else {
        duplicateRemoved.push(item);
      }
    });

    setVariantName(duplicateRemoved);
    const newValue : string = duplicateRemoved.map((x:any) => x.host_identifier).join(',');

    // setNodeIdentifier({...nodeIdentifier, nodes: newValue })
    nodeIdentifier.nodes = newValue;
  };

  return (
    <div>
      <FormControl sx={{ m: 1, width: 800 }}>
        <InputLabel id="demo-multiple-checkbox-label">Select Node</InputLabel>
        <Select
          labelId="demo-multiple-checkbox-label"
          id="demo-multiple-checkbox"
          multiple
          value={variantName}
          onChange={handleChange}
          input={<OutlinedInput label="Select Node" />}
          renderValue={(selected) => selected.map((x:any) => x.hostname).join(', ')}
          MenuProps={MenuProps}
          disabled={isDisabled}
        >
          {data.filter((node:any) => node.status.includes("Online")).map((variant:any) => (
            <MenuItem key={variant.id} value={variant}>
              <Checkbox
                checked={
                  variantName.findIndex((item:any) => item.id === variant.id) >= 0
                }
              />
              <ListItemText primary={variant.hostname + " [Online] " + variant.os_name} />
            </MenuItem>
          ))}
        </Select>
      </FormControl>
    </div>
  );
}


// Checkbox id issue pending..
export default NodeSelectDropdown;