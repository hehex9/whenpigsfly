Import.style("./style.styl");

let component = React.statelessComponent("Empty");

let make = children => {
  ...component,

  render: _self => <div className="empty"> ...children </div>,
};