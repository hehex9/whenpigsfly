Import.style("./style.styl");

let component = React.statelessComponent("Spinner");

let make = (~loading, _children) => {
  ...component,

  render: _self =>
    loading ?
      <div className="lds-ellipsis"> <div /> <div /> <div /> <div /> </div> :
      React.null,
};