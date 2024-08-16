import { useEffect, useState } from 'react';
import { Alerters } from '../../data/rules-data';
import { default as ReactSelect } from 'react-select';
import { components } from 'react-select';

interface ReactMultiSelectProps{
  handleMultiSelectChange : any
  value: any
}

const ReactMultiSelectDropdown = ({handleMultiSelectChange, value}:ReactMultiSelectProps) => {
  const [optionSelected, setOptionsSelected] = useState();

  useEffect(() => {
    setOptionsSelected(value)
  }, [value])

  const Option = (props: any) => {
    return (
      <div>
        <components.Option {...props}>
          <input
            type="checkbox"
            className="form-check-input"
            checked={props.isSelected}
            onChange={() => null}
          />{' '}
          <label className="form-check-label">{props.label}</label>
        </components.Option>
      </div>
    );
  };

  const handleInputChange = (selected: any) => {
    setOptionsSelected(selected);
    handleMultiSelectChange(selected)
  };

  return (
    <ReactSelect
      className="multiple-select"
      classNamePrefix="multiple-select-element"
      options={Alerters}
      isMulti
      blurInputOnSelect={false}
      closeMenuOnSelect={false}
      hideSelectedOptions={false}
      //   menuIsOpen={true}
      placeholder="Nothing Selected"
      components={{
        Option,
      }}
      isSearchable={true}
      onChange={handleInputChange}
      value={optionSelected}
    />
  );
};

export default ReactMultiSelectDropdown;
